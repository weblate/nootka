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

#include "tscorebeam.h"
#include "tscorenote.h"
#include "tscorestaff.h"
#include "tnoteitem.h"
#include "tscoremeasure.h"
#include <music/tnote.h>
#include <QtWidgets/qgraphicsscene.h>
#include <QtCore/qdebug.h>


/**
 * Simple structure to describe second beam line (sixteenth)
 * which can be chunked when there are rests or eights in the group
 */
class T16beam {
public:
  T16beam(int firstStemNr = 0, QGraphicsPolygonItem* beamPoly = nullptr) : startStem(firstStemNr), b(beamPoly) {}
  ~T16beam() { delete b; }
  int startStem = -1; /**< Undefined by default */
  int endStem = -1; /**< When remains undefined - beam is partial */

      /** @p TRUE when beam is not connected to another stem */
  bool isHalf() { return endStem == -1; }
  QGraphicsPolygonItem* b = nullptr; /**< beam item */
};


#define MIN_STEM_HEIGHT (4) // minimal stem height (distance of note head to staff boundary)
#define STEM_HEIGHT (5.8)
#define SLOPER (1.5) // common value to change first and last stems length when there are more than two notes in a beam
#define HALF_STEM (0.1) // half of stem line width - this is also distance that stem line takes above stem points
#define BEAM_THICK (0.8) // thickness of a beam


TscoreBeam::TscoreBeam(TscoreNote* sn, TscoreMeasure* m) :
  QObject(m),
  m_measure(m)
{
  sn->note()->rtm.setBeam(Trhythm::e_beamStart);
  addNote(sn);
  qDebug() << "     [BEAM] created for note id" << sn->index();
}


TscoreBeam::~TscoreBeam()
{
  qDebug() << "     [BEAM] deleted of id" << first()->index();
  for (TscoreNote* note : m_notes) {
    note->note()->rtm.setBeam(Trhythm::e_noBeam); // restore beams
    note->setBeam(nullptr);
  }
  qDeleteAll(m_16_beams);
  delete m_8_beam;
}


void TscoreBeam::addNote(TscoreNote* sn) {
  if (m_notes.isEmpty() && sn->note()->rtm.beam() != Trhythm::e_beamStart) // TODO: delete it when no errors
    qDebug() << "     [BEAM] new beam starts but not proper flag is set!";
  else if (!m_notes.isEmpty() && last()->note()->rtm.beam() == Trhythm::e_beamEnd)
    qDebug() << "     [BEAM] Adding note to beam group that already ended!";

  if (m_notes.isEmpty())
    m_summaryPos = 0;
  m_summaryPos += sn->newNotePos();

  m_notes << sn;
  sn->setBeam(this);
//   if (sn->newRhythm()->beam() == Trhythm::e_beamEnd)
  connect(sn, &TscoreNote::noteWasClicked, this, &TscoreBeam::performBeaming);

// 'main' beam of eights - always exists
  if (sn->note()->rtm.beam() == Trhythm::e_beamStart) { // resume grouping, prepare for painting
    m_8_beam = createBeam(sn);
  }

// beam of 16th - it may be chunked when rest or eight occurs
  if (sn->note()->rhythm() == Trhythm::e_sixteenth) {
    if (sn->note()->isRest()) {

    } else {
        if (m_16_beams.isEmpty() || m_notes.at(m_notes.count() - 2)->note()->rhythm() != Trhythm::e_sixteenth
           || (m_notes.at(m_notes.count() - 2)->note()->rhythm() == Trhythm::e_sixteenth && m_notes.at(m_notes.count() - 2)->note()->isRest())) {
        // is first in beam  or previous note was not a sixteenth or it was sixteenth but rest
              m_16_beams << new T16beam(m_notes.count() - 1, createBeam(sn)); // then create new beam segment
        } else if (!m_16_beams.isEmpty() && m_notes.at(m_notes.count() - 2)->note()->rhythm() == Trhythm::e_sixteenth
                   && !m_notes.at(m_notes.count() - 2)->note()->isRest()) {
        // there is 16 beam and previous note was 16th and not a rest
              m_16_beams.last()->endStem = m_notes.count() - 1; // then set end stem for it
        }
    }
  }

}


void TscoreBeam::closeBeam() {
  last()->note()->rtm.setBeam(Trhythm::e_beamEnd);
  connect(m_measure, &TscoreMeasure::updateBeams, this, &TscoreBeam::beamsUpdateSlot);

  if (first()->note()->rtm.beam() == Trhythm::e_beamStart && last()->note()->rtm.beam() == Trhythm::e_beamEnd)
    qDebug() << "     [BEAM] closed correctly with" << count() << "notes";
  else
    qDebug() << "     [BEAM] is corrupted!!!!";
}

//#################################################################################################
//###################              PROTECTED           ############################################
//#################################################################################################
void TscoreBeam::beamsUpdateSlot(TscoreNote* sn) {
  if (!sn || (sn->index() >= first()->index() && sn->index() <= last()->index()))
    performBeaming();
  else
    qDebug() << "     [BEAM] ignored beaming of" << sn->index();
}


/**
 * This method (slot) is invoked after TscoreNote is updated and placed apparently to the rhythm in measure (staff).
 * TscoreNote::noteWasClicked() signal invokes it actually
 *
 * With @p stemDirStrength (strength of stem direction) we trying to determine preferred common direction of stems in the group.
 * More far from middle of the staff (18) note is placed - stronger its direction has to be preserved
 * to keep beaming more natural and looking good
 * 
 * @p stemsUpPossible keep true when there is enough space between note head and staff border for whole stem
 * @p firstStemOff and @p lastStemOff are calculated to make appropriate stems longer
 * and keep beam slope nice and avoid collisions with notes in between first and last ones.
 *
 * Then @p drawBeam() is called.
 *
 * TODO: WHAT ABOUT PIANO STAFF WHERE ARE TWO MIDDLE LINES
*/
void TscoreBeam::performBeaming() {
// find common stem direction for beaming
  int stemDirStrength = 0;
  bool stemsUpPossible = true;
  qint16 hiNote = 99, loNote = 0;
  for (TscoreNote* sn : m_notes) {
      stemDirStrength += sn->notePos() - 18;
      if (sn->notePos() < MIN_STEM_HEIGHT)
          stemsUpPossible = false;
      hiNote = qMin(hiNote, sn->notePos());
      loNote = qMax(loNote, sn->notePos());
  }

  if (last()->note()->rtm.beam() != Trhythm::e_beamEnd)
      qDebug() << "     [BEAM] was not closed!!" << m_notes.size();

  bool allStemsDown = !stemsUpPossible;
  if (stemDirStrength < 0)
    allStemsDown = true; // stems down are always possible

  qreal firstStemOff = 0.0, lastStemOff = 0.0;
  // pick up or pull down fist and/or last stems to keep beam above all notes and avoid collision of middle note(s) and beam
  if (allStemsDown) {
      firstStemOff = qMax(qreal(loNote - first()->notePos()) - SLOPER, 0.0);
      lastStemOff = qMax(qreal(loNote - last()->notePos()) - SLOPER, 0.0);
  } else {
      firstStemOff = qMax(qreal(first()->notePos() - hiNote) - SLOPER, 0.0);
      lastStemOff = qMax(qreal(last()->notePos() - hiNote) - SLOPER, 0.0);
  }

// initial setting stems directions and stem lines
  for (int i = 0; i < count(); ++i) {
    auto n = m_notes[i]; // cache pointer for multiple reuse
    Trhythm r(n->note()->rtm);
    r.setStemDown(allStemsDown);
    n->setRhythm(r);
    // set only first and last stems - the inner ones later - adjusted to leading beam line
    if (n == last() || n == first()) {
      if (n->note()->rtm.stemDown())
        n->mainNote()->setStemLength((n->notePos() < n->staff()->height() - STEM_HEIGHT ? STEM_HEIGHT : 3.5)
                                    + (n == first() ? firstStemOff : 0.0)
                                    + (n == last() ? lastStemOff : 0.0));
      else
        n->mainNote()->setStemLength(-((n->notePos() < STEM_HEIGHT ? 3.5 : STEM_HEIGHT) - 0.85)
                                    - (n == first() ? firstStemOff : 0.0)
                                    - (n == last() ? lastStemOff : 0.0));
    }
  }

  qDebug() << "     [BEAM" << first()->index() << "]" << "beaming was done" << stemDirStrength << m_16_beams.count();

  drawBeam();
}

/**
 * @p beamLine is calculated as a base line, inner stems fitted to it.
 * If there are some sixteenths - @p m_16_beams are painted
 */
void TscoreBeam::drawBeam() {
  QPointF stemOff(0.0, last()->note()->rtm.stemDown() ? HALF_STEM : -HALF_STEM); // offset to cover stem line thickness
  QLineF beamLine(first()->pos() + first()->mainNote()->stemEndPoint() + stemOff, last()->pos() + last()->mainNote()->stemEndPoint() + stemOff);
// adjust stem lines length to leading beam line for notes in between of the beam group
  for (int i = 1; i < m_notes.count() - 1; ++i) {
      QPointF stemEnd;
      auto s = m_notes[i]->stem();
      auto nPos = m_notes[i]->pos() + m_notes[i]->mainNote()->pos();
      beamLine.intersect(QLineF(nPos + s->line().p1(), nPos + s->line().p2()), &stemEnd);
      s->setLine(QLineF(s->line().p1(), stemEnd - nPos - 2 * stemOff));
  }
  QPolygonF poly;
  poly << beamLine.p1() << beamLine.p2();
      /** @p beamOff the lower point on a stem for bottom beam outline (it depends on beam line angel) */
  QPointF beamOff(0.0, (last()->note()->rtm.stemDown() ? -BEAM_THICK : BEAM_THICK) / qCos(qDegreesToRadians(beamLine.angle())));
  applyBeam(poly, beamOff, m_8_beam);

  if (!m_16_beams.isEmpty()) {
    QPointF beam16LineOff = beamOff * 1.5;
    beamLine.translate(beam16LineOff);
    for (T16beam* b16 : m_16_beams) {
        poly.clear();
        poly << m_notes[b16->startStem]->pos() + m_notes[b16->startStem]->mainNote()->stemEndPoint() + stemOff + beam16LineOff;
        if (b16->isHalf()) { // 16th beam of fist stem is right-sided others are left-sided
            QPointF halfPoint;
            qreal halfX = poly.last().x() + BEAM_THICK * (b16->startStem == 0 ? 2 : -2);
            // intersection point with fake stem line for chunked beam
            beamLine.intersect(QLineF(halfX, poly.last().y(), halfX, poly.last().y() - 6.0), &halfPoint);
            poly << halfPoint;
        } else
            poly << m_notes[b16->endStem]->pos() + m_notes[b16->endStem]->mainNote()->stemEndPoint()  + stemOff + beam16LineOff;
        applyBeam(poly, beamOff, b16->b);
    }
  }
  qDebug() << "     [BEAM" << first()->index() << "]" << "DRAW BEAM";
}


void TscoreBeam::changeStaff(TscoreStaff* st) {
  for (T16beam* b16 : m_16_beams)
    b16->b->setParentItem(st);
  m_8_beam->setParentItem(st);
}


//#################################################################################################
//###################              PRIVATE             ############################################
//#################################################################################################

void TscoreBeam::applyBeam(QPolygonF& poly, const QPointF& offset, QGraphicsPolygonItem* polyItem) {
  poly << poly.last() + offset;
  poly << poly.first() + offset;
  poly << poly.first();
  polyItem->setPolygon(poly);
}


QGraphicsPolygonItem* TscoreBeam::createBeam(TscoreNote* sn) {
  QGraphicsPolygonItem* b = new QGraphicsPolygonItem(sn->staff());
  b->setPen(Qt::NoPen);
  b->setBrush(sn->mainNote()->color());
  b->setZValue(55);
  return b;
}


