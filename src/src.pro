#=================================================================
# This file is part of Nootka (http://nootka.sf.net)
# Copyright (C) 2017-2020 by Tomasz Bojczuk (seelook@gmail.com)
# on the terms of GNU GPLv3 license (http://www.gnu.org/licenses)
#
# Main Nootka executable
#=================================================================


QT += multimedia androidextras core widgets quick quickcontrols2 printsupport


TARGET = Nootka
TEMPLATE = app

SOURCES +=  main.cpp \
            mobile/tmobilemenu.cpp \
          \
            main/tnameitem.cpp \
            main/texamexecutor.cpp \
            main/texecutorsupply.cpp \
            main/tglobalexamstore.cpp \
            main/texercises.cpp \
            main/tequalrand.cpp \
            main/texammelody.cpp \
            main/trandmelody.cpp \
            main/texamview.cpp \
            main/tpenalty.cpp \
            main/ttiphandler.cpp \
            main/tmainscoreobject.cpp \
            main/texamsummary.cpp \
            main/tstartexamitem.cpp \
            main/tnootkacertificate.cpp \
            main/tpiechartitem.cpp \
            main/tnotetoplay.cpp \
            main/tgotit.cpp \
          \
            dialogs/tdialogloaderobject.cpp \
            dialogs/tlevelcreatoritem.cpp \
            dialogs/tlevelpreviewitem.cpp \
            dialogs/tlevelsdefs.cpp \
            dialogs/tlevelselector.cpp \
            dialogs/tmelodylistview.cpp \
            dialogs/tmelodywrapper.cpp \
            dialogs/ttunerdialogitem.cpp \
            dialogs/trtmselectoritem.cpp \
            dialogs/tmelgenitem.cpp \
          \
            help/texamhelp.cpp \
            help/tmainhelp.cpp \
          \
            updater/tupdatechecker.cpp \
            updater/updatefunctions.cpp \
            updater/tupdateruleswdg.cpp \
            updater/tupdatesummary.cpp \
          \

HEADERS  += mobile/tmobilemenu.h \
          \
            main/tnameitem.h \
            main/texamexecutor.h \
            main/texecutorsupply.h \
            main/tglobalexamstore.h \
            main/texercises.h \
            main/tequalrand.h \
            main/texammelody.h \
            main/trandmelody.h \
            main/texamview.h \
            main/tpenalty.h \
            main/ttiphandler.h \
            main/tmainscoreobject.h \
            main/texamsummary.h \
            main/tstartexamitem.h \
            main/tnootkacertificate.h \
            main/tpiechartitem.h \
            main/tnotetoplay.h \
            main/tgotit.h \
          \
            dialogs/tdialogloaderobject.h \
            dialogs/tlevelcreatoritem.h \
            dialogs/tlevelpreviewitem.h \
            dialogs/tlevelsdefs.h \
            dialogs/tlevelselector.h \
            dialogs/tmelodylistview.h \
            dialogs/tmelodywrapper.h \
            dialogs/ttunerdialogitem.h \
            dialogs/trtmselectoritem.h \
            dialogs/tmelgenitem.h \
          \
            help/texamhelp.h \
            help/tmainhelp.h \
          \
            updater/tupdatechecker.h \
            updater/updatefunctions.h \
            updater/tupdateruleswdg.h \
            updater/tupdatesummary.h \
          \



CONFIG += mobility warn_off
MOBILITY = 

android {
  QMAKE_CXXFLAGS_RELEASE += -fsigned-char
  QMAKE_CXXFLAGS_DEBUG += -fsigned-char -O1

  versionAtLeast(QT_VERSION, 5.15.0) {
    DEFINES += QT_NO_DEPRECATED_WARNINGS
    # Nootka core & sound libraries are added automatically
  } else {
    ANDROID_EXTRA_LIBS += $$OUT_PWD/libs/core/libNootkaCore.so \
                          $$OUT_PWD/libs/sound/libNootkaSound.so \
  }

}

INCLUDEPATH += libs/core libs/mobile libs/sound libs/main

versionAtLeast(QT_VERSION, 5.15.0) {
  LIBS += -Llibs/core/ -lNootkaCore_$${QT_ARCH} \
          -Llibs/sound -lNootkaSound_$${QT_ARCH} \
} else {
  LIBS += -Llibs/core/ -lNootkaCore \
          -Llibs/sound -lNootkaSound \
}

RESOURCES += nootka-android.qrc

versionAtLeast(QT_VERSION, 5.15.0) {
  DISTFILES += \
    android21/AndroidManifest.xml \
    android21/res/values/libs.xml \
    android21/build.gradle \
    android21/gradle/wrapper/gradle-wrapper.jar \
    android21/gradlew \
    android21/gradle/wrapper/gradle-wrapper.properties \
    android21/gradlew.bat \
    android21/net/sf/nootka/TshareExam.java \
    android21/net/sf/nootka/ToutVolume.java \

  ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android21
} else {
  DISTFILES += \
    android/AndroidManifest.xml \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/net/sf/nootka/TshareExam.java \
    android/net/sf/nootka/ToutVolume.java \

  ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

# append Qt base translations from current Qt installation
TR_DIR = "$$system(dirname $$QMAKESPEC)/../translations"

lang.path = /assets/lang
lang.files += lang/nootka_cs.qm
lang.files += lang/nootka_de.qm
lang.files += lang/nootka_es.qm
lang.files += lang/nootka_fr.qm
lang.files += lang/nootka_hu.qm
lang.files += lang/nootka_it.qm
lang.files += lang/nootka_pl.qm
lang.files += lang/nootka_ru.qm
lang.files += lang/nootka_sl.qm
lang.files += lang/qtbase_sl.qm
lang.files += lang/nootka_uk.qm
lang.files += $$system(ls $$TR_DIR/qtbase_cs.qm)
lang.files += $$system(ls $$TR_DIR/qtbase_de.qm)
lang.files += $$system(ls $$TR_DIR/qtbase_es.qm)
lang.files += $$system(ls $$TR_DIR/qtbase_fr.qm)
lang.files += $$system(ls $$TR_DIR/qtbase_hu.qm)
lang.files += $$system(ls $$TR_DIR/qtbase_it.qm)
lang.files += $$system(ls $$TR_DIR/qtbase_pl.qm)
lang.files += $$system(ls $$TR_DIR/qtbase_ru.qm)
lang.files += $$system(ls $$TR_DIR/qtbase_uk.qm)

lang.depends += FORCE

INSTALLS += lang
