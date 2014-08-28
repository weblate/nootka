/***************************************************************************
 *   Copyright (C) 2014 by Tomasz Bojczuk                                  *
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

#ifndef TCHUNK_H
#define TCHUNK_H

#include "tnote.h"
#include "trhythm.h"
#include <tfingerpos.h>

class QXmlStreamReader;
class QXmlStreamWriter;


/** 
 * This class represent a note: 
 * a pitch described by @p Tnote 
 * and its value (relative duration) described by @p Trhythm
 */
class Tchunk
{

public:
	Tchunk(const Tnote& pitch, const Trhythm& rhythm, const TfingerPos& fretPos = TfingerPos());
	~Tchunk();
	
	Tnote& p() { return m_pitch; } /** The pitch of a note */
	TfingerPos& g() { return m_fretPos; } /** Position a note on a guitar (if any) - by default it is invalid */
	Trhythm& r() { return m_rhythm; } /** rhythm value of a note */

			/** Returns @p TRUE when position on the guitar is valid. */
	bool validPos() { if (g().str() == 7) return false; else return true; }
	
	void toXml(QXmlStreamWriter& xml);
	bool fromXml(QXmlStreamReader& xml);
	
	
private:
	Tnote				m_pitch;
	Trhythm			m_rhythm;
	TfingerPos  m_fretPos;
};

#endif // TCHUNK_H
