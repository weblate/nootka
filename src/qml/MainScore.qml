/***************************************************************************
 *   Copyright (C) 2017 by Tomasz Bojczuk                                  *
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

import QtQuick 2.7
import QtQuick.Controls 2.0

import Score 1.0
import Nootka 1.0


Score {
  id: mainScore

  width: parent.width

  property alias showNamesAct: showNamesAct
  property alias extraAccidsAct: extraAccidsAct
  property alias zoomInAct: zoomInAct
  property alias zoomOutAct: zoomOutAct

  enableKeySign: true
  scoreObj.nameColor: GLOB.nameColor
  scoreObj.showNoteNames: GLOB.namesOnScore

  Rectangle { // note highlight
    id: noteHighlight
    parent: currentNote
    visible: currentNote != null
    width: currentNote ? (currentNote.width - currentNote.alterWidth) * 1.5 : 0
    height: currentNote ? Math.min(12.0, currentNote.notePosY + 6.0) : 0
    x: currentNote ? -width * 0.25 : 0
    y: currentNote ? Math.min(currentNote.height - height, Math.max(0.0, currentNote.notePosY - height / 2.0)) : 0
    color: Qt.rgba(activPal.highlight.r, activPal.highlight.g, activPal.highlight.b, 0.3)
    z: -1
    radius: width / 3.0
  }

  Taction {
    id: extraAccidsAct
    text: qsTr("Additional accidentals")
    checkable: true
  }
  Taction {
    id: showNamesAct
    text: qsTr("Show note names")
    checkable: true
    checked: GLOB.namesOnScore
    onTriggered: { scoreObj.showNoteNames = checked }
  }
  Taction {
    id: zoomOutAct
    icon: "zoom-out"
    text: qsTr("Zoom score out")
    onTriggered: scaleFactor = Math.max(0.4, scaleFactor - 0.2)
    shortcut: Shortcut { sequence: StandardKey.ZoomOut; onActivated: zoomOutAct.triggered() }
  }
  Taction {
    id: zoomInAct
    icon: "zoom-in"
    text: qsTr("Zoom score in")
    onTriggered: scaleFactor = scaleFactor = Math.min(scaleFactor + 0.2, 1.4)
    shortcut: Shortcut { sequence: StandardKey.ZoomIn; onActivated: zoomInAct.triggered() }
  }

  Shortcut {
    sequence: StandardKey.MoveToNextChar;
    onActivated: {
      if (currentNote) {
          if (currentNote.index < notesCount - 1)
            currentNote =  scoreObj.note(currentNote.index + 1)
      } else
          currentNote = scoreObj.note(0)
      console.log(portableText)
    }
  }
  Shortcut {
    sequence: StandardKey.MoveToPreviousChar;
    onActivated: {
      if (currentNote) {
          if (currentNote.index > 0)
            currentNote = scoreObj.note(currentNote.index - 1)
      } else {
          currentNote = scoreObj.note(0)
      }
      console.log(portableText)
    }
  }

}
