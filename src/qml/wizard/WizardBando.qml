/** This file is part of Nootka (http://nootka.sf.net)               *
 * Copyright (C) 2018-2021 by Tomasz Bojczuk (seelook@gmail.com)     *
 * on the terms of GNU GPLv3 license (http://www.gnu.org/licenses)   */

import QtQuick 2.9


Column {
  spacing: NOO.factor()
  anchors.horizontalCenter: parent.horizontalCenter
  width: parent.width * 0.9

  Text {
    text: "Bandoneon support is experimental and not well tested,\nso please give some feedback."
    color: "red"
    width: parent.width
    horizontalAlignment: Text.AlignHCenter
    font { pixelSize: NOO.factor() * 1.5; bold: true }
  }
}
