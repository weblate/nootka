/***************************************************************************
 *   Copyright (C) 2013-2014 by Tomasz Bojczuk                             *
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

#ifndef TMAINSCORE_H
#define TMAINSCORE_H

#include "tmultiscore.h"
#include <QPointer>

class Tmelody;
class TcornerProxy;
class TscoreActions;
class TscoreKeys;
class QShortcut;
class QTimer;
class TpushButton;
class QToolBar;
class QAction;
class TnoteName;
class TscoreNote;
class TgraphicsTextTip;
class TblinkingItem;
class TstrikedOutItem;
class QGraphicsRectItem;
class QGraphicsTextItem;
class QGraphicsSimpleTextItem;

/** 
 * This is a main score of Nootka.
 * In exam mode it responses for locking/unlocking, backgrounds, question marks.
 */
class TmainScore : public TmultiScore
{
    Q_OBJECT

public:
    TmainScore(QMainWindow* mw, QWidget* parent = 0);
    ~TmainScore();
		
		
		void setEnableEnharmNotes(bool isEnabled);
    void acceptSettings();
		
		int widthToHeight(int hi); /** Returns width of score when its height is @p hi. */
		
    virtual void setNote(const Tnote& note);
    virtual void setNote(int index, const Tnote& note) { TmultiScore::setNote(index, note); }
		void setMelody(Tmelody* mel);
		void getMelody(Tmelody* mel, const QString& title = ""); /** Grabs all notes, key and time in a score to given @p mel */
		
		void setInsertMode(EinMode mode);
		
				/** Describes moving of selected note */
		enum EmoveNote {
			e_nextNote, e_prevNote, e_nextStaff, e_prevStaff, e_last, e_first, e_doNotMove
		};		

    void askQuestion(Tnote note, char realStr = 0);
    void askQuestion(Tnote note, TkeySignature key, char realStr = 0);
		void askQuestion(Tmelody* mel);
    void clearScore();
		
				/** It sets TkeySignatureView background to question color, sets fake key signature
        * and invokes askQuestion in TkeySignatureView */
    void prepareKeyToAnswer(TkeySignature fakeKey, QString expectKeyName);
		
        /** Connects or disconnects reactions for clicking a note
        * and showing enharmonic notes depends on is exam executing (disconnect)
        * or not (connect).*/
    void isExamExecuting(bool isIt);
		bool isExam() { return (bool)m_questMark; } /** @p True during exam/exercises */
        /** Internally it calls setScoreDisabled(false) to unlock and locks unused noteViews (1 & 2) again. */
    void unLockScore();
		virtual void setScoreDisabled(bool disabled);
    void setKeyViewBg(QColor C);
    void setNoteViewBg(int id, QColor C);
    void forceAccidental(Tnote::Ealter accid); /** Sets given accidental */
		
        /** Marks note with border and blur. It is used to show was answer correct or not. Default note number for single mode. */
    void markAnswered(QColor blurColor, int noteNr = 0);
    void markQuestion(QColor blurColor, int noteNr = 1);
		
				/** When @p TRUE notes on locked score can be selected by click 
				 * and @p lockedNoteClicked(Tnote) signal is emitted */
		void setReadOnlyReacted(bool doIt);
		
		void enableAccidToKeyAnim(bool enable);
		bool isAccidToKeyAnimEnabled();
		
				/** Performs animation that transforming current selected note to given @p goodNote */
		void correctNote(Tnote& goodNote, const QColor& color, int noteNr = 0);
		
				/** Performs rewinding of current key to @p newKey */
		void correctKeySignature(TkeySignature newKey);
		void correctAccidental(Tnote& goodNote);
		
				/** Displays note name of first note or the second if given next to its note-head in a score. */
		void showNames(Tnote::EnameStyle st);
		void deleteNoteName(int id); /** Deletes given instance of note name if it exist */
		
				/** Returns note head rectangle if visible or empty QRectF.  */
		QRectF noteRect(int noteNr);
			
				/** Position of a note in graphics view coordinates. */
		QPoint notePos(int noteNr);
		
		bool isScorePlayed() { return m_scoreIsPlayed; }
		TnoteName* noteName() { return m_nameMenu; }
		
		bool isCorrectAnimPending() { return m_correctNoteNr > -1; } /** @p TRUE when animation is in progress. */
		
signals:
		void noteChanged(int index, Tnote note);
		
    void noteClicked(); /** This signal is emitted during an exam when expert answers are used. */
		void playbackFinished();
		
		void lockedNoteClicked(int noteNumber); /** Emitted number is in range [0 to notesCount()] */
		void lockedNoteSelected(int noteNumber); /** Emitted number is in range [0 to notesCount()] */
		
public slots:
    void whenNoteWasChanged(int index, Tnote note);
		void noteWasClickedMain(int index);
    void setScordature(); /** Sets scordature to value kept in Tglobal. */
		void expertNoteChanged();
		void onClefChanged(Tclef cl);
		void playScore(); /** Plays (actually emits noteChanged()) all notes starting from the selected one. */

protected:
	virtual void resizeEvent(QResizeEvent* event);
		
protected slots:
		void strikeBlinkingFinished();
		void keyBlinkingFinished();
		void finishCorrection();
		void zoomScoreSlot();
		
		void showNameMenu(TscoreNote* sn);
		void menuChangedNote(Tnote n);
		void extraAccidsSlot();
		void showNamesSlot();
		void moveSelectedNote(EmoveNote nDir = e_doNotMove);
		void moveNameForward() { moveName(e_nextNote); }
		void moveNameBack() { moveName(e_prevNote); }
		
		void playSlot();
		void roClickedSlot(TscoreNote* sn);
		void roSelectedSlot(TscoreNote* sn);
		
private:
		void restoreNotesSettings(); /** Sets notes colors according to globals. */
		void performScordatureSet(); /** Common method to set scordature */
		void createQuestionMark();
		
    virtual void addStaff(TscoreStaff* st = 0);
		
				/** Creates QGraphicsRectItem with answer color, places it under the staff and adds to m_bgRects list.
				 * clearScore() removes it. */
		void createBgRect(QColor c, qreal width, QPointF pos);
		
		void setBarsIconSize();
		void createActions();
		void moveName(EmoveNote moveDir);
		void createNoteName();
		void enableCorners(bool enable); /** Activates/deactivates corner menus. */

private:
		QGraphicsSimpleTextItem 		*m_questMark;
		QGraphicsTextItem 					*m_questKey;
		QList<QGraphicsRectItem*> 	 m_bgRects; // list of rectangles with highlights
		TstrikedOutItem 						*m_strikeOut;
    QPointer<TblinkingItem>      m_bliking, m_keyBlinking;
		int													 m_correctNoteNr;
		Tnote												 m_goodNote;
		TkeySignature								 m_goodKey;
		Tnote::EnameStyle						 m_corrStyle;
		QToolBar										*m_settBar, *m_clearBar, *m_rhythmBar;
		
		TcornerProxy 								*m_settCorner, *m_delCorner, *m_rhythmCorner;
		TnoteName					 					*m_nameMenu;
		QPointer<TscoreNote>				 m_currentNameSegment; /** Currently edited TscoreNote by menu. */
		bool												 m_scoreIsPlayed;
		QPointer<QTimer>						 m_playTimer;
		TscoreKeys									*m_keys; /** Score shortcuts */
		TscoreActions								*m_acts; /** Score actions (tool bars icons/buttons) */
		int 												 m_nameClickCounter, m_playedIndex;
		bool												 m_selectReadOnly;
};

#endif // TMAINSCORE_H
