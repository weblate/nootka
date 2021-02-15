/** This file is part of Nootka (http://nootka.sf.net)               *
 * Copyright (C) 2017-2021 by Tomasz Bojczuk (seelook@gmail.com)     *
 * on the terms of GNU GPLv3 license (http://www.gnu.org/licenses)   */

import QtQuick 2.9
import QtQuick.Controls 2.2

import Nootka 1.0
import "../"


Rectangle {
  property string helpText: dialLoader.dialogObj.mainHelp()
  property alias enableTOC: tocButt.visible

  width: parent.width; height: parent.height
  color: Noo.alpha(activPal.base, 240)
  z: 0
  Image {
    source: Noo.pix(images[currTopic])
    height: parent.height; width: height * (sourceSize.width / sourceSize.height)
    z: -1
    anchors.horizontalCenter: parent.horizontalCenter
  }

  TcuteButton {
    id: tocButt
    y: Noo.factor() / 4; x: parent.width - width - Noo.factor()
    z: 10
    text: qsTr("Help topics")
    onClicked: tocMenu.open()
  }

  StackView {
    id: stack
    anchors.fill: parent
    initialItem: mainHelp
  }

  Component {
    id: mainHelp
    Tflickable {
      y: (tocButt.visible ? tocButt.height : 0) + Noo.factor()
      width: parent ? parent.width : 0; height: parent ? parent.height - y : 0
      contentHeight: text.height;
      LinkText {
        id: text
        width: parent.width
        padding: Noo.factor()
        wrapMode: TextEdit.Wrap; textFormat: Text.RichText
        text: helpText
      }
    }
  }

  property var topics: [ qsTranslate("ThelpDialogBase", "Nootka help"),
                          qsTranslate("TstartExamDlg", "To exercise or to pass an exam?"),
                          qsTranslate("TexamHelp", "How does an exercise or an exam work?"),
                          Noo.TR("MainMenuMobile", "Pitch recognition"),
                          qsTranslate("ThelpDialogBase", "Open online documentation")
                       ]
  property var images: [ "help", "startExam", "nootka-exam", "pane/sound", "restore-defaults" ]
  property int currTopic: 0
  property var gotItQML: [ "", "ExamOrExercise", "ExamFlow", "SoundInfo" ]

  Component.onCompleted: {
    if (Noo.isAndroid()) {
      topics.splice(4, 0, Noo.TR("HandleScore", "Editing score with touch"))
      images.splice(4, 0, "pane/score")
      gotItQML.splice(4, 0, "HandleScore")
    }
    if (enableTOC)
      tocRep.model = topics
  }

  Tmenu {
    id: tocMenu
    width: Noo.factor() * 25
    x: parent.width - width - y; y: Noo.factor() / 2
    Repeater {
      id: tocRep
      MenuItem {
        padding: 0
        contentItem: MenuButton {
          action: Taction {
            text: (currTopic === index ? "<u>" : "") + modelData + (currTopic === index ? "</u>" : "")
            icon: images[index]
          }
          onClicked: switchTopic(index)
          Rectangle { width: parent.width; height: index === topics.length - 1 ? 0 : 1; color: activPal.text; y: parent.height - 1 }
        }
      }
    }
  }

  function switchTopic(tp) {
    if (tp !== currTopic) {
      if (tp === topics.length - 1) {
          Qt.openUrlExternally("https://nootka.sourceforge.io/index.php/help/")
      } else {
          if (tp > 0)
            stack.replace(Qt.createComponent("qrc:/gotit/" + gotItQML[tp] + ".qml")
                 .createObject(stack, { "visible": false, "showGotIt": false }).contentItem)
          else
            stack.replace(mainHelp)
          currTopic = tp
      }
    }
    tocMenu.close()
  }
}
