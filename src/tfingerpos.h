/***************************************************************************
 *   Copyright (C) 2011 - 2012 by Tomasz Bojczuk                           *
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


#ifndef TFINGERPOS_H
#define TFINGERPOS_H

#include <QDataStream>


/**
*  A class describes finger's position on the fingerboard.
*/

class TfingerPos
{
public:
    TfingerPos() { m_pos = 255; }
    TfingerPos(unsigned char realStr, unsigned char fret) {
        setPos(realStr, fret);
    }

    quint8 str() { return (m_pos / 40) +1; }
    quint8 fret() { return m_pos % 40; }

    void setPos(unsigned char realStr, unsigned char fret) {
        m_pos = (realStr-1)*40 + fret;
    }
        /** List of QStrings with roman representatin of numbers 0-24. */
    static const QString fretsList[25];
    static QString romanFret(quint8 fret);
    QString romanFret() { return romanFret(fret()); }
      /** TfingerPos in HTML format as a string fe.: 3 XVII */
    QString toHtml(TfingerPos pos);

    bool operator==( TfingerPos f2) { return m_pos == f2.m_pos; }
    bool operator!=( TfingerPos f2) { return m_pos != f2.m_pos; }

    friend QDataStream &operator<< (QDataStream &out, const TfingerPos &fPos); 
    friend QDataStream &operator>> (QDataStream &in, TfingerPos &fPos);

protected:
    quint8 m_pos;
};



#endif // TFINGERPOS_H
