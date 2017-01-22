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
 *  You should have received a copy of the GNU General Public License	     *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/

import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.0


/**
* Implements a dialog with navigation list on the left
* Model has following fields:
* 'icon', 'text' and 'page' - with path to qml file
*/
Item {
//  anchors.fill: parent

  property alias model: navList.model
  property alias stack: stack
  property HeadButton prevButt: null

  Rectangle { color: activPal.base; x: navList.x; y: navList.y; width: navList.width; height: navList.height }

  Row {
    anchors { fill: parent }

    // navigation list on the left
    ListView {
      id: navList
      clip: true
      height: parent.height
      property var buttons: []

      delegate: HeadButton {
        id: delegateButt
        property var pageItem
        anchors.horizontalCenter: parent.Center
        name: buttonText
        icon: Tpath.pix(iconName)
        factor: 1.5 * Screen.pixelDensity
        fontSize: nootkaWindow.font.pixelSize
        onClicked: {
          if (stack.index !== index) {
            stack.currentIndex = index
            if (prevButt)
              prevButt.color = "transparent"
            prevButt = delegateButt
            prevButt.color = activPal.highlight
          }
        }
        Component.onCompleted: {
          var w = Math.max(navList.width, width)
          navList.width = w
          navList.buttons.push(delegateButt)
          for (var i = 0; i < navList.buttons.length; ++i) // center all buttons
            navList.buttons[i].width = w
          if (index === 0) {
            prevButt = delegateButt
            prevButt.color = activPal.highlight
          }
        }
      }
      ScrollBar.vertical: ScrollBar {}
    }

    Item { width: Screen.pixelDensity * 2; height: parent.height } // spacer

    // pages container on the right
    StackLayout {
      id: stack
      width: parent.width - navList.width - Screen.pixelDensity * 2
//      Layout.fillHeight: true
      height: parent.height
//      anchors { left: navList.Right; margins: Screen.pixelDensity * 2 }
      currentIndex: 0
    }
  }
}
 
