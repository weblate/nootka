/** This file is part of Nootka (http://nootka.sf.net)               *
 * Copyright (C) 2021 by Tomasz Bojczuk (seelook@gmail.com)          *
 * on the terms of GNU GPLv3 license (http://www.gnu.org/licenses)   */

import QtQuick 2.9

import Nootka.Main 1.0
//import "../score"
import "../"


GotIt {
  id: gotImport
  gotItType: TgotIt.GotImportScore
  visible: true

  onClicked: {
    if (showGotIt) { // Got It, not help dialog
      if (!remaindChecked) // store only if user doesn't want it next time
        GLOB.setGotIt("ScoreImport", false)
    }
    close()
    destroy()
  }

  background: Rectangle {
    gradient: Gradient {
      GradientStop { position: 0.0; color: "transparent" }
      GradientStop { position: textTop / gotImport.height - 0.1; color: "transparent" }
      GradientStop { position: textTop / gotImport.height + 0.05; color: Qt.tint(activPal.base, NOO.alpha(activPal.highlight, 50)) }
    }
  }

  // private
  property real textTop: gotImport.height - Math.min(impCol.height + NOO.factor() * (NOO.isAndroid() ? 3 : 9), gotImport.height * 0.7)
  Tflickable {
    width: parent.width; height: parent.height * 0.7
    y: textTop
    contentHeight: impCol.height + NOO.factor() * 4; contentWidth: width

    Column {
      id: impCol
      spacing: NOO.factor(); leftPadding: NOO.factor()
      width: parent.width - NOO.factor() * 13
      Text {
        width: parent.width
        wrapMode: Text.WordWrap
        anchors.horizontalCenter: parent.horizontalCenter
        horizontalAlignment: Text.AlignHCenter
        text: qsTr("Unfortunately Nootka cannot handle all notes of this score at once.")
        color: activPal.text
        font { bold: true; pixelSize: NOO.factor() * 1.5 }
      }
      Text {
        width: parent.width
        wrapMode: Text.WordWrap
        text: qsTr("But you can select here some voice or a part and import it to the application.") + "<br>"
            + qsTr("Before that you can transform (split, transpose and etc.) score parts and also select which note of a chord will be imported.")
        color: activPal.text
      }
      Text {
        width: parent.width
        wrapMode: Text.WordWrap
        text: qsTr("When you are preparing an exam or an exercise from the score parts, multiple fragments can be imported at once.")
        color: activPal.text
      }
    }
  }

}