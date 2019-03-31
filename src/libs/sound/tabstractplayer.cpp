/***************************************************************************
 *   Copyright (C) 2013-2019 by Tomasz Bojczuk                             *
 *   seelook@gmail.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/


#include "tabstractplayer.h"
#include "toggscale.h"
#include <music/tnote.h>
#include <music/tmelody.h>
#include <taudioparams.h>
#include <tglobals.h>
#include <tpath.h>

#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"

#include <QtCore/qthread.h>
#include <QtCore/qfile.h>
#include <QtCore/qdatastream.h>

#include <QtCore/qdebug.h>
#include "checktime.h"


unsigned int            TabstractPlayer::p_posInNote = 0;
unsigned int            TabstractPlayer::p_posInOgg = 0;
int                     TabstractPlayer::p_playingNoteNr = -1;
int                     TabstractPlayer::p_decodingNoteNr = -1;
int                     TabstractPlayer::p_playingNoteId = -1;
int                     TabstractPlayer::p_prevNote = -100;
unsigned int            TabstractPlayer::p_shiftOfPrev = 0;
unsigned int            TabstractPlayer::p_lastPosOfPrev = 0;

unsigned int            TabstractPlayer::p_beatPeriod = 0;
unsigned int            TabstractPlayer::p_beatBytes = 7984; // beat file frames number (initial, finally obtained from file)
unsigned int            TabstractPlayer::p_beatOffset = 0;


TabstractPlayer::TabstractPlayer(QObject* parent) :
  QObject(parent),
  p_playable(false),
  p_doEmit(false)
{
  m_thread = new QThread();
  moveToThread(m_thread);
  connect(m_thread, &QThread::started, this, &TabstractPlayer::playThreadSlot);
}


TabstractPlayer::~TabstractPlayer() {
  if (m_thread->isRunning()) {
    m_thread->terminate();
    m_thread->wait();
  }
  delete m_thread;
  if (m_beatArray)
    delete[] m_beatArray;
}


bool TabstractPlayer::playNote(int noteNr) {
  if (!p_playable)
    return false;
  m_thread->wait();
  m_noteToPlay = noteNr;
  m_thread->start();
  return true;
}


bool TabstractPlayer::playNotes(QList<Tnote>* notes, int tempo, int firstNote) {
  if (!p_playable)
    return false;
  m_thread->wait();
  m_listToPlay = notes;
  p_tempo = tempo;
  m_firstNote = firstNote;
  m_thread->start();
  return true;
}


bool TabstractPlayer::playMelody(Tmelody* melody, int transposition) {
  if (!p_playable)
    return false;
  m_thread->wait();
  m_melodyToPlay = melody;
  m_transposition = transposition;
  m_thread->start();
  return true;
}


/**
 * Reading metronome beat from ogg file takes really less, so let's do it this way.
 * It should take a few ms only on low end Android gizmo.
 * But of course, read it once, and store decoded data in array.
 * TODO: consider to save decoded data somewhere into cache.
 */
void TabstractPlayer::runMetronome(unsigned int beatTempo) {
  if (!m_beatArray) {
    auto stdFile = fopen(Tpath::sound("beat").toStdString().c_str(), "r");
    if(!stdFile) {
      qDebug() << "[TabstractPlayer] Can't open metronome beat file";
      p_beatBytes = 0;
      return;
    }
    OggVorbis_File oggFile;
    ov_open(stdFile, &oggFile, NULL, 0);
    m_beatArray = new qint16[20000];
    int bitStream;
    long bytes;
    p_beatBytes = 0;
    do {
      bytes = ov_read(&oggFile, reinterpret_cast<char*>(m_beatArray) + p_beatBytes, 20000 - p_beatBytes, 0, 2, 1, &bitStream);
      p_beatBytes += bytes;
    } while (bytes > 0);
    p_beatBytes = p_beatBytes / 2;
// Below dead code reads from raw audio (48000Hz/16bits) file
//     p_beatBytes = ov_read(&oggFile, reinterpret_cast<char*>(m_beatArray), 20000, 0, 2, 1, &bitStream) / 2;
//     QFile beatFile(Tpath::sound("beat", ".raw"));
//     if (!beatFile.exists()) {
//       p_beatPeriod = 0; // disable metronome
//       return;
//     }
//     beatFile.open(QIODevice::ReadOnly);
//     QDataStream beatStream(&beatFile);
//     m_beatArray = new qint16[beatFile.size() / 2];
//     p_beatBytes = beatStream.readRawData(reinterpret_cast<char*>(m_beatArray), beatFile.size()) / 2;
//     beatFile.close();
    ov_clear(&oggFile);
  }
  p_beatOffset = 0;
  p_beatPeriod = beatTempo ? (44100 * 60) / beatTempo : 0;
}


void TabstractPlayer::stopMetronome() {
}


//#################################################################################################
//###################                PROTECTED         ############################################
//#################################################################################################

void TabstractPlayer::preparePlayList(QList<Tnote>* notes, int tempo, int firstNote, int sampleRate, int transposition, int a440diff) {
  playList().clear();
  for (int n = firstNote; n < notes->count(); ++n) {
    const Tnote& tmpN = notes->at(n);
    int samplesDuration = qRound(((tmpN.duration() > 0 ? tmpN.duration() / 24.0 : 1.0) * (60000.0 / tempo)) * (sampleRate / 1000.0));
    if (tmpN.rtm.tie() > Trhythm::e_tieStart) { // append duration if tie is continued or at end
      if (playList().isEmpty())
        continue; // do not start playing in the middle of tied notes
        playList().last().samplesCount += samplesDuration;
    } else
        playList() << TsingleSound(n, tmpN.isValid() ? tmpN.chromatic() + transposition + a440diff : REST_NR, samplesDuration);
  }
}


void TabstractPlayer::playThreadSlot() {
  if (!m_playList.isEmpty() && p_playingNoteNr <= m_playList.size() - 1 && p_posInNote < m_playList[p_playingNoteNr].samplesCount) {
      // something still is playing
      p_prevNote = m_playList[p_playingNoteNr].number;
  } else {
      p_prevNote = -100;
      p_shiftOfPrev = 0;
      p_lastPosOfPrev = 0;
  }
  p_playingNoteNr = 0;
  p_decodingNoteNr = 0;
  p_doEmit = true;
  m_playList.clear();
  if (m_noteToPlay != REST_NR) { // single note
      playList() << TsingleSound(0, m_noteToPlay + static_cast<int>(p_audioParams->a440diff), qRound(p_oggScale->sampleRate() * 1.5));
      m_noteToPlay = REST_NR;
  } else if (m_listToPlay) { // note list (i.e. score notes)
      preparePlayList(m_listToPlay, p_tempo, m_firstNote, p_oggScale->sampleRate(), GLOB->transposition(), static_cast<int>(p_audioParams->a440diff));
      m_listToPlay = nullptr;
  } else if (m_melodyToPlay) { // melody (from Tmelody)
      for (int n = 0; n < m_melodyToPlay->length(); ++n) { // TODO: !! Where is beat unit of melody !!!!!!
        const Tnote& tmpN = m_melodyToPlay->note(n)->p();
        int samplesDuration = qRound(((tmpN.duration() > 0 ? tmpN.duration() / 24.0 : 1.0) * (60000.0 / m_melodyToPlay->tempo())) * (p_oggScale->sampleRate() / 1000.0));
        if (tmpN.rtm.tie() > Trhythm::e_tieStart) { // append duration if tie is continued or at end
            if (playList().isEmpty())
              continue; // do not start playing in the middle of tied notes
              playList().last().samplesCount += samplesDuration;
        } else
            playList() << TsingleSound(n,
                                       tmpN.isValid() ? tmpN.chromatic() + GLOB->transposition() + m_transposition + p_audioParams->a440diff : REST_NR,
                                       samplesDuration);
      }
      m_melodyToPlay = nullptr;
  }
  if (!playList().isEmpty()) { // naughty user might want to play tied note at the score end
    p_playingNoteId = playList().first().id;
    startPlaying();
  }
  m_thread->quit();
}




