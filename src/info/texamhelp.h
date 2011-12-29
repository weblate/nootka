/***************************************************************************
 *   Copyright (C) 2011 by Tomasz Bojczuk                                  *
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

#ifndef TEXAMHELP_H
#define TEXAMHELP_H

#include <QDialog>

class QLabel;

/** It shows simple exam help. The parameters are QStrings with colors 
 * in HTML format: 
 * background-color: rgba() 
 * @param path is global path to images*/
class TexamHelp : public QDialog
{
  Q_OBJECT
  
public:
  TexamHelp(QString questColorTxt, QString answColorTxt, QString &path, QWidget *parent = 0);
  static QString orRightButtTxt() { return tr("or right mouse button"); }
};

#endif // TEXAMHELP_H
