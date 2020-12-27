/** This file is part of Nootka (http://nootka.sf.net)               *
 * Copyright (C) 2017-2020 by Tomasz Bojczuk (seelook@gmail.com)     *
 * on the terms of GNU GPLv3 license (http://www.gnu.org/licenses)   */

import QtQuick 2.9

import Nootka.Dialogs 1.0
import "../"


TlevelPreviewItem {
  id: levelPrev

  Rectangle {
    anchors.fill: parent
    color: activPal.base
    Text {
      color: Qt.tint(activPal.base, Noo.alpha(activPal.highlight, 50))
      font { family: "Nootka"; pixelSize: parent.width * 0.75 }
      anchors.centerIn: parent
      text: instrumentGlyph
    }
    MouseArea {
      id: viewArea
      anchors.fill: parent
      onWheel: {
        if (wheel.modifiers & Qt.ControlModifier) {
            if (wheel.angleDelta.y > 0)
              zoom(true)
            else if (wheel.angleDelta.y < 0)
              zoom(false)
        } else
            wheel.accepted = false
      }
    }
  }

  property real maxLabelWidth: Noo.fontSize() * 10
  property real maxValueWidth: Noo.fontSize() * 10

  Tflickable {
    anchors.fill: parent
    contentWidth: Math.max(width, width * levCol.scale)
    contentHeight: levCol.height * levCol.scale
    Column {
      id: levCol
      topPadding: Noo.fontSize()
      anchors.horizontalCenter: parent.horizontalCenter
      transformOrigin: Item.Top

      PreviewItem {
        textItem2.horizontalAlignment: Text.AlignHCenter
        text2: header
      }

      Item { width: 10; height: Noo.fontSize() }

      Rectangle {
        visible: validLevel
        width: levRow.width; height: Noo.fontSize() / 6
        color: activPal.text
        anchors.horizontalCenter: parent.horizontalCenter
      }

      Row {
        id: levRow
        visible: validLevel
        anchors.horizontalCenter: parent.horizontalCenter
        Rectangle { width: Noo.fontSize() / 6; height: levRow.height; color: activPal.text }

        Column {
          PreviewItem { text2: instrument }
          Rectangle { width: parent.width; height: Noo.fontSize() / 12; color: activPal.text }

          PreviewItem {
            id: noteRangeIt
            handleWidth: true
            height: Noo.fontSize() * 2
            text: Noo.TR("RangePage", "note range:").replace(":", " ")
            text2: noteRange
            textItem2.font { family: "Scorek"; pixelSize: Noo.fontSize() * 1.6 }
            textItem2.y: height * -0.9
          }

          PreviewItem {
            id: fretRangeIt
            handleWidth: true
            text: Noo.TR("RangePage", "fret range:").replace(":", " ")
            text2: fretRange
          }

          PreviewItem {
            id: keyRangeIt
            handleWidth: true
            text: qsTranslate("TlevelPreviewItem", "key signature:").replace(":", " ")
            text2: keyRange
          }

          PreviewItem {
            id: accidentalsIt
            handleWidth: true
            text: Noo.TR("AccidsPage", "accidentals") + " "
            text2: accidentals === "" ? qsTranslate("TlevelPreviewItem", "none") : accidentals
            textItem2.font { family: accidentals === "" ? "default" : "Nootka"; pixelSize: Noo.fontSize() * (accidentals === "" ? 1.2 : 1.8) }
          }

          PreviewItem {
            handleWidth: true
            text: Noo.TR("LevelCreator", "Questions") + " "
            text2: questions
            textItem2.font { family: "Nootka"; pixelSize: Noo.fontSize() * 2.2 }
          }

          PreviewItem {
            handleWidth: true
            text: qsTranslate("LevelCreator", "Answers") + " "
            text2: answers
            textItem2.font { family: "Nootka"; pixelSize: Noo.fontSize() * 2.2 }
          }

          PreviewItem {
            text: " "
            text2: requireOctave
          }
        } // Column

        Rectangle { width: Noo.fontSize() / 6; height: levRow.height; color: activPal.text }

        Column {
          PreviewItem {
            layHorizontal: false
            text: "<br>" + qsTranslate("TlevelPreviewItem", "Clef") + ":"
            textItem2.font { family: "Scorek"; pixelSize: Noo.fontSize() * 4 }
            text2: clef
          }
        }

        Rectangle { width: Noo.fontSize() / 6; height: levRow.height; color: activPal.text }
      } // levRow Row

      Rectangle {
        visible: validLevel
        width: levRow.width; height: Noo.fontSize() / 6
        color: activPal.text
        anchors.horizontalCenter: parent.horizontalCenter
      }

      Item { width: 10; height: Noo.fontSize() }

      PreviewItem {
        visible: validLevel
        text: " "
        textItem2.width: levelPrev.width * 0.9 / levCol.scale
        textItem2.wrapMode: Text.WordWrap
        text2: description
      }
    } // levCol Column
  }

  Timer { id: zoomTimer; interval: 100 }

  Row {
    anchors { right: parent.right; bottom: parent.bottom }
    HeadButton {
      factor: Noo.fontSize() / 3; hiHover: false
      height: factor * 9.5
      pixmap: Noo.pix("zoom-out")
      onClicked: zoom(false)
    }
    HeadButton {
      factor: Noo.fontSize() / 3; hiHover: false
      height: factor * 9.5
      pixmap: Noo.pix("zoom-in")
      onClicked: zoom(true)
    }
  }

  function zoom(zoomIn) {
    if (!zoomTimer.running) {
      var sc = levCol.scale * (zoomIn ? 1.0625 : 0.9375)
      if (sc < 0.5 || sc > 2.0)
        return
      levCol.scale = sc
      zoomTimer.running = true
    }
  }
}
