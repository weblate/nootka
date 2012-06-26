/***************************************************************************
 *   Copyright (C) 2012 by Tomasz Bojczuk                                  *
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



#ifndef TDIALOGMESSAGE_H
#define TDIALOGMESSAGE_H

#include <QDialog>
#include "tnote.h"

class TkeySignature;
class Texam;
class MainWindow;
class TQAunit;
class TexamLevel;
class QLabel;



class TdialogMessage : public QDialog
{
   Q_OBJECT
public:
    explicit TdialogMessage(Texam *exam, MainWindow *parent, Tnote::EnameStyle style);
    static QString getTextHowAccid(Tnote::Eacidentals accid);
    static QString getKeyText(TkeySignature &key);
        /** Returns html-formated question text. */
    QString getQuestion(TQAunit &question, int questNr, TexamLevel *level, Tnote::EnameStyle style);
    
public slots:
    void mainWindowMoved(QPoint vector);
    void mainWindowMaximised();
    void mainWindowMinimised();
    void mainWindowResized();
	
protected:
    virtual void paintEvent(QPaintEvent *);
    
    void setPosAndSize();
    
private:
    QLabel *m_mainLab;
    QPoint m_pos;
    bool m_scoreFree, m_nameFree, m_guitarFree; // Indicate when widgets show question or answer
    MainWindow *m_parent;
};

#endif // TDIALOGMESSAGE_H
