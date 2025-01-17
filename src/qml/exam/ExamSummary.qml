/** This file is part of Nootka (http://nootka.sf.net)               *
 * Copyright (C) 2018-2021 by Tomasz Bojczuk (seelook@gmail.com)     *
 * on the terms of GNU GPLv3 license (http://www.gnu.org/licenses)   */

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0

import Nootka.Exam 1.0
import "../level"
import "../"


TexamSummary {
  id: summDialog
  width: parent.width; height: parent.height
  levelPreview: previewItem

  Column {

    Grid {
      columns: 2
      padding: NOO.factor() / 2
      Tflickable {
        height: summDialog.height - buttGrid.height - summDialog.width / 50
        width: summDialog.width / 2 - NOO.factor() / 2
        contentHeight: summCol.height
        Column {
          id: summCol
          width: parent.width
          Text { // student
            anchors.horizontalCenter: parent.horizontalCenter
            text: student; textFormat: Text.StyledText
          }
          Item { width: NOO.factor(); height: NOO.factor() }
          Tile { // answers/mistakes numbers
            width: parent.width - NOO.factor()
            Grid {
              anchors.horizontalCenter: parent.horizontalCenter
              columns: NOO.isAndroid() ? 1 : 2
              horizontalItemAlignment: Grid.AlignHCenter; verticalItemAlignment: Grid.AlignVCenter
              spacing: NOO.factor() * (NOO.isAndroid() ? 1 : 2)
              Column {
                spacing: NOO.factor()
                Text {
                  anchors.horizontalCenter: parent.horizontalCenter
                  horizontalAlignment: Text.AlignHCenter
                  text: resultHeader; textFormat: Text.StyledText
                  font.pixelSize: NOO.factor() * 1.3
                }
                Grid {
                  columns: 2; columnSpacing: NOO.factor()
                  Repeater {
                    model: answersLabel
                    Text {
                      text: modelData
                      color: activPal.text; textFormat: Text.StyledText
                      font.pixelSize: NOO.factor() * 1.2
                    }
                  }
                }
              }
              Item {
                width: NOO.factor() * (NOO.isAndroid() ? 15 : 20); height: width
                TpieChartItem {
                  id: answId
                  anchors.fill: parent
                  values: answersModel
                  colors: [ cn(GLOB.correctColor), cn(GLOB.notBadColor), cn(GLOB.wrongColor) ]
                }
                DropShadow {
                  anchors.fill: answId
                  horizontalOffset: NOO.factor() / 2; verticalOffset: NOO.factor() / 2
                  radius: NOO.factor()
                  samples: 1 + radius * 2; color: activPal.shadow
                  source: answId
                }
              }
            }
          }
          Tile { // results
            width: parent.width - NOO.factor()
            visible: resultsModel.length
            Grid {
              anchors.horizontalCenter: parent.horizontalCenter
              columns: NOO.isAndroid() ? 1 : 2
              horizontalItemAlignment: Grid.AlignHCenter; verticalItemAlignment: Grid.AlignVCenter
              spacing: NOO.factor() * (NOO.isAndroid() ? 1 : 2)
              Column {
                spacing: NOO.factor()
                Text {
                  anchors.horizontalCenter: parent.horizontalCenter
                  text: qsTranslate("TexamSummary", "Kinds of mistakes") + ":"; color: activPal.text
                }
                Grid {
                  columns: 2; columnSpacing: NOO.factor()
                  Repeater {
                    model: resultsModel
                    Text {
                      text: modelData
                      color: activPal.text; textFormat: Text.StyledText
                    }
                  }
                }
              }
              Item {
                visible: hasVariousMistakes
                width: NOO.factor() * (NOO.isAndroid() ? 15 : 20); height: width
                TpieChartItem {
                  id: pie
                  anchors.fill: parent
                  values: summDialog.kindOfMistakes
                  colors: [ cn(GLOB.wrongColor) ]
                }
                DropShadow {
                  anchors.fill: pie
                  horizontalOffset: NOO.factor() / 2; verticalOffset: NOO.factor() / 2
                  radius: NOO.factor()
                  samples: 1 + radius * 2; color: activPal.shadow
                  source: pie
                }
              }
            }
          }
        }
      }
      Column {
        width: summDialog.width / 2
        Tile { // times
          id: timeTile
          width: parent.width - NOO.factor()
          Column {
            spacing: NOO.factor()
            width: parent.width
            Text {
              anchors.horizontalCenter: parent.horizontalCenter
              horizontalAlignment: Text.AlignHCenter
              text: qsTranslate("TexamSummary", "times:")
              font.pixelSize: NOO.factor() * 1.1
            }
            Grid {
              anchors.horizontalCenter: parent.horizontalCenter
              columns: 2; columnSpacing: NOO.factor()
              Repeater {
                model: timesModel
                Text {
                  text: modelData
                  color: activPal.text; textFormat: Text.StyledText
                }
              }
            }
          }
        }
        LevelPreview {
          id: previewItem
          width: summDialog.width / 2 - NOO.factor() / 2
          height: summDialog.height - buttGrid.height - summDialog.width / 50 - timeTile.height
        }
      }
    }

    Grid {
      id: buttGrid
      anchors.horizontalCenter: parent.horizontalCenter
      spacing: summDialog.width / 100
      columns: buttColumsCount()
      property real buttWidth: buttColumsCount() === 2 ? summDialog.width / 3 : summDialog.width / 4
        TiconButton {
          visible: enableContinue()
          width: buttGrid.buttWidth
          pixmap: NOO.pix(isExercise() ? "practice" : "exam"); iconHeight: summDialog.height / 15
          text: NOO.TR("QWizard", "Continue")
          onClicked: { continueExecutor(); dialLoader.close() }
        }
        TiconButton {
          visible: !NOO.isAndroid() || !isExercise()
          width: buttGrid.buttWidth
          pixmap: NOO.pix(NOO.isAndroid() ? "send" : "charts")
          iconHeight: summDialog.height / 15
          text: NOO.isAndroid() ? NOO.TR("QShortcut", "Send") : qsTr("Analyze")
          onClicked: {
            if (NOO.isAndroid()) {
                sendExam()
            } else {
                nootkaWindow.showDialog(6) // 6 - Nootka.Charts
                nootkaWindow.analyzeWindow.allowOpen = false
                nootkaWindow.analyzeWindow.exam = summDialog.exam()
            }
          }
        }
        TiconButton {
          visible: isExercise()
          width: buttGrid.buttWidth
          pixmap: NOO.pix("exam"); iconHeight: summDialog.height / 15
          text: qsTr("Pass an exam")
          onClicked: { exerciseToExam(); dialLoader.close() }
        }
        TiconButton {
          width: buttGrid.buttWidth
          pixmap: NOO.pix("exit"); iconHeight: summDialog.height / 15
          text: NOO.TR("QPlatformTheme", "Close")
          onClicked: dialLoader.close()
        }
    }
  }

  Component.onCompleted: {
    dialLoader.standardButtons = 0
    dialLoader.title = summDialog.title()
  }

  Component.onDestruction: {
    if (discardedAtBegin()) {
        nootkaWindow.executor.destroy()
        GLOB.isExam = false
    } else
        closeSummary()
  }
}
