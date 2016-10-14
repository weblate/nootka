/***************************************************************************
 *   Copyright (C) 2016 by Tomasz Bojczuk                                  *
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

#ifndef TSCOREMEASURE_H
#define TSCOREMEASURE_H

#include <nootkacoreglobal.h>
#include <QtCore/qobject.h>


class TscoreMeter;
class TscoreStaff;
class TscoreNote;
class TscoreBeam;
class QGraphicsLineItem;
class Tmeter;
class Tnote;
class Trhythm;


/**
 * Implementation of the measure
 */
class NOOTKACORE_EXPORT TscoreMeasure : public QObject
{

  friend class TscoreStaff;

  Q_OBJECT

public:
  explicit TscoreMeasure(TscoreStaff* staff, int nr);
  virtual ~TscoreMeasure();

  void changeMeter(const Tmeter& m);

      /**
       * Sets parent staff and parent object as well
       */
  void setStaff(TscoreStaff* st);

  void insertNote(int id, TscoreNote* sn);

      /**
       * Inserts list of notes at the beginning of this measure.
       * IT IS WASTING ENERGY TO ADDING WHOLE MEASURE THIS WAY!
       * Use this method only for lists of notes shorter than measure duration
       */
  void prependNotes(QList<TscoreNote*>& nl);

      /**
       * Adds list of notes @p nl at the measure end.
       * Measure has to have already space for them
       */
  void appendNotes(QList<TscoreNote*>& nl);

      /**
       * Removes note with index @p noteToRemove, 
       * Asks the staff for notes from the next measure
       * to fulfill free space
       */
  void removeNote(int noteToRemove);

  QList<TscoreNote*>& notes() { return m_notes; }

  bool isEmpty() { return m_notes.isEmpty(); }

  TscoreNote* firstNote() { return m_notes.first(); }

  TscoreNote* lastNote() { return m_notes.last(); }

      /** Staff index of the first measure note */
  int firstNoteId() const;

      /** Staff index of the last measure note */
  int lastNoteId() const;

      /** @p TRUE when there is no more space in the measure */
  bool isFull() const { return m_free == 0; }

      /** Free 'rhythm space' remained in the measure */
  int free() const { return m_free; }

      /** Summary of notes duration starting from @p noteId to the end of measure */
  int freeAt(int noteId) const;

      /** Duration of the measure */
  int duration() const { return m_duration; }

      /** Duration of notes starting from measure begin until @p noteId  */
  int durationAt(int noteId) const;

  Tmeter* meter() const;
  TscoreMeter* scoreMeter() const;

      /**
       * Measure index - number in current staff
       * Starts from 0 in every staff.
       */
  int id() const { return m_id; }

      /**
       * Width of all notes in this measure.
       * Difference between last note position with its rhythm gap
       * and X coordinate of the first note
       */
  qreal notesWidth();

      /** Prints debug message with [nr MEASURE] */
  char debug();

      /**
       * Returns summary duration of notes in the list.
       */
  static int groupDuration(const QList<TscoreNote*>& notes);


signals:

      /**
       * Emitted when measure was changed and re-beamed.
       * TscoreNote parameter is a note that invoked change (or 0 if new one was added)
       * so all other beam groups that don't contain this note has to be updated
       */
  void updateBeams(TscoreNote*);


protected:

      /**
       * Returns first @c TscoreNote in given rhythmic group @p grNr or null pointer if none
       */
  TscoreNote* firstInGroup(int grNr) { return m_firstInGr[grNr] >= 0 ? m_notes[m_firstInGr[grNr]] : nullptr; }

  void noteChangedSlot(TscoreNote* sn);

      /** Only class TscoreStaff can do this */
  void setId(int id) { m_id = id; }

      /**
       * Sets beams in measure notes starting from beam group of @p firstGroup
       * till @p endGroup  or through all groups if not set.
       */
  void resolveBeaming(int firstGroup, int endGroup = -1);

      /** Sets appropriate @p setRhythmGroup of every note in the measure. */
  void updateRhythmicGroups();

      /**
       * Takes @p dur (duration) of notes at the measure end
       * and creates a list of notes @p notesToOut to shift to the next measure.
       * Also, if the last note has to be split
       * - into @p newNote is put Tnote to be shifted at next measure beginning
       * and tied with the last note of this measure.
       * @p endNote is note number till taking out is performed (by default 0 - whole measure)
       * Returned value is remaining duration.
       */
  int releaseAtEnd(int dur, QList<TscoreNote*>& notesToOut, Tnote& newNote, int endNote = 0);

      /**
       * Takes @p dur (duration) of notes at the measure beginning
       * and packs them into @p notesToOut list.
       */
  int takeAtStart(int dur, QList<TscoreNote*>& notesToOut);

      /** Checks to display or hide the bar line */
  void checkBarLine();

private:

      /**
       * Adds @p newNote with index +1 than the last note
       * by invoking @p TscoreStaff::insertNote() 
       */
  void addNewNote(Tnote& newNote);

      /**
       * Common routine that performs:
       * - shifting @p notesToOut to the next measure (if not empty)
       * and creating new, tied note @p newNote at the beginning of the next measure 
       */
  void shiftReleased(QList<TscoreNote*>& notesToOut, Tnote& newNote);

      /**
       * Common method calling the staff for notes from the next measure
       * to fill this one. @p m_free has to be updated before.
       */
  void fill();

      /**
       * Splits given note into two (using @p Trhythm::split()).
       * It adds that second note to the staff and this measure
       * and ties with given @p sn note
       */
  void split(TscoreNote* sn);

      /** Splits given duration @p dur into two notes of @p note and adds them to the staff at @p id */
  void splitThenInsert(int dur, int id, const Tnote& note, bool readOnly);

  void fixStemDirection(TscoreNote* sn);

      /** Restores ties of @p thisNote by given @p tieCopy value of @p Trhythm::Etie*/
  void restoreTie(quint8 tieCopy, TscoreNote* thisNote);

  void copyRhythmParams(TscoreNote* sn, const Trhythm& r);

private:
  int                      m_duration;
  int                      m_id;
  int                      m_free;
  TscoreStaff             *m_staff; /**< Parent staff */
  QList<TscoreNote*>       m_notes;
  QList<TscoreBeam*>       m_beams;
  QGraphicsLineItem       *m_barLine;
  qint8                   *m_firstInGr; /**< quint8 is sufficient - measure never has more than 127 notes */
};


#endif // TSCOREMEASURE_H
