/** This file is part of Nootka (http://nootka.sf.net)               *
 * Copyright (C) 2017 by Tomasz Bojczuk (seelook@gmail.com)          *
 * on the terms of GNU GPLv3 license (http://www.gnu.org/licenses)   */

import QtQuick 2.7

import Score 1.0


Flickable {
  id: score

  property alias scoreObj: scoreObj
  property alias scale: staff0.scale
  property alias firstStaff: staff0
  property int clef: Tclef.Treble_G_8down
  property alias meter: scoreObj.meter
  property alias bgColor: bgRect.color
  property bool enableKeySign: false
  property bool enableDoubleAccids: false
  property real scaleFactor: 1.0
  property alias notesCount: scoreObj.notesCount
  property TnoteItem currentNote: null

  // private
  property var staves: []

  clip: true
  width: parent.width

  contentWidth: score.width

  TscoreObject {
    id: scoreObj
    width: score.width / scale
    cursorAlter: accidControl.alter
    enableDoubleAccidentals: score.enableDoubleAccids

    onClicked: score.currentNote = scoreObj.activeNote

    onStaffCreate: {
      var c = Qt.createComponent("qrc:/Staff.qml")
      var lastStaff = c.createObject(score.contentItem, { "clef.type": score.clef })
      staves.push(lastStaff)
      lastStaff.enableKeySignature(enableKeySign)
      //       score.contentY = score.contentHeight - score.height TODO ensure visible
      lastStaff.keySignature.onKeySignatureChanged.connect(setKeySignature)
      lastStaff.onDestroing.connect(removeStaff)
      if (enableKeySign)
        lastStaff.keySignature.key = staff0.keySignature.key
    }
    onStavesHeightChanged: score.contentHeight = Math.max(stavesHeight, score.height)

    function removeStaff(nr) { staves.splice(nr, 1) }
  }

  Rectangle { // entire score background
    id: bgRect
    anchors.fill: score.contentItem
    color: activPal.base
  }

  Staff {
    id: staff0
    clef.type: score.clef
    meter: Meter { parent: staff0 }
    clef.onTypeChanged: {
      // TODO: approve clef for all staves
    }
    Component.onCompleted: staves.push(staff0)
    Component.onDestruction: destroing("destroying staff")
  }

  NoteCursor {
    id: cursor
    parent: scoreObj.activeNote
    anchors.fill: parent
    yPos: scoreObj.activeYpos
    upperLine: scoreObj.upperLine
    alterText: accidControl.text
    z: 20
  }

  AccidControl {
    id: accidControl
    active: scoreObj.activeNote !== null
  }

  onEnableKeySignChanged: {
    staff0.enableKeySignature(enableKeySign)
    if (enableKeySign)
      staff0.keySignature.onKeySignatureChanged.connect(setKeySignature)
    for (var s = 1; s < staves.length; ++s) {
      staves[s].enableKeySignature(enableKeySign)
      if (enableKeySign)
        staff0.keySignature.onKeySignatureChanged.connect(setKeySignature)
    }
    scoreObj.keySignatureEnabled = enableKeySign
  }

  function setKeySignature(key) {
    if (enableKeySign) {
      for (var s = 0; s < staves.length; ++s) {
        if (key !== staves[s].keySignature.key)
          staves[s].keySignature.key = key
      }
      scoreObj.keySignature = key
    }
  }

  function addNote(n) { scoreObj.addNote(n) }
  function setNote(staff, nr, n) { scoreObj.setNote(staff, nr, n) }
}
