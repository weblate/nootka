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

#ifndef TCLEFSELECTOR_H
#define TCLEFSELECTOR_H

#include "tscoreitem.h"
#include "tclef.h"

class TclefPreview;
class TscoreScene;

/** This class implements QGraphicsObject which appeaars  when user clicked on clef.
 * User gets possibility to select a clef or piano staff. */
class TclefSelector : public TscoreItem
{
    Q_OBJECT

public:
    TclefSelector(TscoreScene *scene);

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    virtual QRectF boundingRect() const;
		
signals:
		void clefSelected(Tclef);

protected slots:
    void clefClicked(Tclef clef);

private:
		TclefPreview *m_treble, *m_treble_8, *m_bass, *m_bass_8, *m_alto, *m_tenor, *m_piano;
		void createEntry(TscoreScene *scene, TclefPreview* &clefView, Tclef clef);

};

#endif // TCLEFSELECTOR_H
