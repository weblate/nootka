/***************************************************************************
 *   Copyright (C) 2011-2013 by Tomasz Bojczuk                             *
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


#ifndef TEXAMVIEW_H
#define TEXAMVIEW_H

#include <QWidget>
#include <QTime>
#include <QTimer>

class TQAunit;
class QLabel;


    /** A @class TexamView represents status of exam.
    * It displays times and numbers of valid/invalid questions.
    */
class TexamView : public QWidget
{
    Q_OBJECT
public:
    explicit TexamView(QWidget *parent = 0);
		
      /** Returns time given in milisec. in format h:mm:ss */
    static QString formatedTotalTime(int t) { return QString("%1:%2:%3")
            .arg(t/3600000)
            .arg((t%3600000)/60000, 2, 'f', 0, '0')
            .arg((t%60000)/1000, 2, 'f', 0, '0'); }

    void startExam( int passTimeInSec = 0, int questNumber = 0,int averTime = 0, int mistakes = 0, int halfMist = 0);
    void questionStart();
    quint16 questionStop();
		
      /** Elapsed time for of current question [seconds * 10] */
    quint16 questionTime();
    void setAnswer(TQAunit *answer = 0);
    void setFontSize(int s);
		
				/** Stops counting time of answer */
		void pause();
		
				/** Continues counting time of answer */
		void go();

    void stopExam() { m_timer->stop(); }
    
    void doNotdisplayTime() { 	m_timer->stop(); } /** Stops displaying pending time */
    void displayTime() { 	m_timer->start(100); } /** Starts Refreshing elapsing time every 100 ms  */
    
        /** This method returns rounded average time. It is only for exam preview.*/
    quint16 getAverageTime() { return (quint16)qRound(m_averTime); }
    quint32 getTotalTime() {return m_totElapsedTime + quint32(m_totalTime.elapsed() / 1000); }
    quint16 getMistakesNumber() {return (quint16)m_mistakes; }
    quint16 getHalfMistakesNr() { return (quint16)m_halfMistakes; }
    int effectiveness() { return qRound(m_effect); }
    void clearResults();
		
        /** Sets background of mistakes/correct answers number Qlabel.
         * Background color is directly inserted to setStyleSheet so 
         * it has to be given in format: 
         * background-color: rgba(red, green, blue, alpha) */
    void setStyleBg(QString okBg, QString wrongBg, QString notBadBg = "");


protected:
    void resizeEvent(QResizeEvent *);
    
private:
    bool 			m_showReact;

    QLabel 		*m_reactTimeLab, *m_averTimeLab, *m_totalTimeLab;
    QLabel 		*m_mistLab, *m_corrLab, *m_effLab, *m_halfLab;
		QTime 		m_reactTime;
    int 			m_questNr, m_mistakes, m_totElapsedTime, m_halfMistakes;
    qreal 		m_averTime, m_effect;
    QTimer 		*m_timer;
    QTime 		m_totalTime;
		int 			m_okCount;
		int 			m_pausedAt; // when m_averTime was paused

private slots:
    void countTime();

};

#endif // TEXAMVIEW_H
