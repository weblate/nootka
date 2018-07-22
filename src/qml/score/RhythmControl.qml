/** This file is part of Nootka (http://nootka.sf.net)               *
 * Copyright (C) 2017-2018 by Tomasz Bojczuk (seelook@gmail.com)     *
 * on the terms of GNU GPLv3 license (http://www.gnu.org/licenses)   */

import QtQuick 2.9

import Score 1.0


ControlBase {
  id: rhythmControl

  parent: score.parent
  x: show ? 1 : -width - Noo.fontSize()
  y: Noo.fontSize() / 2
  visible: !scoreObj.touched

  property string rhythmText: Noo.rhythmText(scoreObj.workRhythm)
  property bool triplet: false
  property bool tie: false

  component: Component {
      id: contentComp
      Column {

        Component {
          id: ctrlButtonComp
          ControlButton {
            property int rhythm: 1
            property bool rest: false
            factor: rhythmControl.factor * 0.9
            yOffset: factor / 2
            font { family: "nootka"; pixelSize: factor * 1.8 }
            text: Noo.rhythmText(Noo.rhythm(rhythm, rest, false, false))
            selected: rhythm === scoreObj.workRtmValue && rest === scoreObj.workRtmRest
            onEntered: hideTimer.stop()
            onExited: hideTimer.restart()
          }
        }

        Grid {
          columns: 2
          Repeater {
            model: 10
            Loader {
              sourceComponent: ctrlButtonComp
              onLoaded: { item.rhythm = 1 + index / 2; item.rest = index % 2 === 0 }
              Connections {
                target: item
                onClicked: { scoreObj.workRtmValue = item.rhythm; scoreObj.workRtmRest = item.rest }
              }
            }
          }
          Loader { // triplet
            id: tripLoad
            sourceComponent: ctrlButtonComp
            onLoaded: { item.rhythm = 0; item.text = "\u0183" }
            Binding { target: tripLoad.item; property: "selected"; value: rhythmControl.triplet }
            Connections {
              target: tripLoad.item
              onClicked: { rhythmControl.triplet = !tripLoad.item.selected; scoreObj.workRhythm = rhythm }
            }
          }
          Loader { // dot
            id: dotLoad
            sourceComponent: ctrlButtonComp
            onLoaded: { item.rhythm = 0; item.text = "." }
            Binding { target: dotLoad.item; property: "selected"; value: scoreObj.workRtmDot }
            Binding { target: dotLoad.item; property: "enabled"; value: scoreObj.workRtmValue !== Trhythm.Sixteenth }
            Connections {
              target: dotLoad.item
              onClicked: scoreObj.workRtmDot = !scoreObj.workRtmDot
            }
          }
        }

//         ControlButton { // tie
//           anchors.horizontalCenter: parent.horizontalCenter
//           factor: rhythmControl.factor * 1.2
//           selected: rhythmControl.tie
//           height: factor * 1.5
//           yOffset: -factor * 1.5
//           font { family: "nootka"; pixelSize: factor * 3.6 }
//           text: "\ue18c"
//           onClicked: rhythmControl.tie = !selected
//           onEntered: hideTimer.stop()
//           onExited: hideTimer.restart()
//         }
      }
  }

  Behavior on x { enabled: GLOB.useAnimations; SpringAnimation { spring: 2; damping: 0.3; duration: 300 }}

}
