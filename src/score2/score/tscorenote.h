/***************************************************************************
 *   Copyright (C) 2013 by Tomasz Bojczuk                                  *
 *   tomaszbojczuk@gmail.com                                               *
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

#ifndef TSCORENOTE_H
#define TSCORENOTE_H

#include "tscoreitem.h"


class TscoreScene;


class TscoreNote : public TscoreItem
{
  Q_OBJECT
  
public:
    TscoreNote(TscoreScene *scene, TscoreStaff *staff, int index);
    ~TscoreNote();
    
        /** Hides main note */
    void hideNote();
        /** Hides pointing (work) note */
    void hideWorkNote();
        /** Sets color of main note. */
    void setColor(QColor color);
        /** Sets color of pointing (work) note. */
    void setPointedColor(QColor color);
    
        /** Adds blur effect to main note. If blurColor is -1 deletes the effect. */
    void markNote(QColor blurColor);    
    void moveNote(int pos);
        /** Min and Max values of Y coefficient on the staff */
    void setAmbitus(int min, int max){ m_ambitMin = min; m_ambitMax = max; }

        /** This return value of @li -2 is bb @li 1 is #
         * @li etc... */
    int accidental() {return m_accidental;}
    int notePos() { return m_mainPosY; }
        /** Returns QString with accidental symbol*/
    static QString getAccid(int accNr);
    static QFont getAccidFont();
    
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    virtual QRectF boundingRect() const;
    
signals:
    void noteWasClicked(int);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void wheelEvent(QGraphicsSceneWheelEvent* event);
    
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event);

private:
    QGraphicsEllipseItem *m_workNote, *m_mainNote;
    QGraphicsSimpleTextItem *m_workAccid, *m_mainAccid;
    QGraphicsLineItem *m_upLines[7], *m_mainUpLines[7], *m_mainDownLines[5], *m_downLines[5];
    QColor m_workColor, m_mainColor;
    
    int m_workPosY, m_mainPosY;
    int m_curentAccid, m_accidental;
    int m_index;
        /** Represents range (ambitus) of notes on score */
    int m_ambitMin, m_ambitMax;
    qreal m_height;
    
private:
        /** Prepares noteHead (elipse) */
    QGraphicsEllipseItem* createNoteHead();
    
};

#endif // TSCORENOTE_H
