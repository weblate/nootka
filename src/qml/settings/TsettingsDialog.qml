/** This file is part of Nootka (http://nootka.sf.net)               *
 * Copyright (C) 2017-2021 by Tomasz Bojczuk (seelook@gmail.com)     *
 * on the terms of GNU GPLv3 license (http://www.gnu.org/licenses)   */

import QtQuick 2.9
import QtQuick.Controls 2.2


Item {
  id: settings

  property int clef: GLOB.clefType
  property int instrument: GLOB.instrument.typeINT

  onInstrumentChanged: {
    if (pages.buttons.length > 2)
      pages.buttons[2].pixmap = NOO.pix("pane/i-0" + instrument)
  }

  width: parent.width
  height: parent.height

  PagesDialog { id: pages }

  Component.onCompleted: {
    if (!GLOB.isExam) {
      pages.addItem("global", qsTr("Common"), "settings/Global")
      pages.addItem("score", qsTr("Score"), "settings/Score")
      pages.addItem("i-0" + GLOB.instrument.typeINT, qsTr("Instrument"), "settings/Instrument")
      pages.addItem("sound", qsTr("Sound"), "settings/Sound")
    }
    pages.addItem("questions", qsTr("Exercises") + "\n& " + qsTr("Exam"), "settings/Exam")
    if (NOO.isAndroid() && !GLOB.isExam)
      pages.addItem("phoneSett", qsTr("Phone") + "\n& " + qsTr("Tablet"), "settings/Phone")

    dialLoader.standardButtons = DialogButtonBox.Apply | DialogButtonBox.Cancel | DialogButtonBox.RestoreDefaults | DialogButtonBox.Help
    dialLoader.title = "Nootka - " + qsTranslate("TsettingsDialog", "application's settings")
  }

  function apply() {
    GLOB.clefType = clef // it can be changed either by score or instrument page
    for (var i = 0; i < pages.pages.length; ++i) {
      if (typeof(pages.pages[i]) === 'object')
        pages.pages[i].save()
    }
  }

  function reset() { pages.currentPage.defaults() }

  function help() { pages.currentPage.help() }
}
