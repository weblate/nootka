; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "Nootka"
!define PRODUCT_VERSION "0.8.95-beta"
!define PRODUCT_PUBLISHER "Nootka"
!define PRODUCT_WEB_SITE "http://nootka.sf.net"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\nootka.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor lzma

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "picts\pack.ico"
!define MUI_UNICON "picts\pack.ico"

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"
!define MUI_LANGDLL_ALLLANGUAGES

; Image
;!define MUI_WELCOMEFINISHPAGE_BITMAP "logo.bmp"
;!define MUI_HEADERIMAGE
;!define MUI_HEADERIMAGE_BITMAP "nootka.bmp"
;!define MUI_HEADERIMAGE_RIGHT

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "gpl"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\nootka.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "Czech"
!insertmacro MUI_LANGUAGE "Polish"
!insertmacro MUI_LANGUAGE "Russian"


; Reserve files
!insertmacro MUI_RESERVEFILE_LANGDLL
;!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Nootka-${PRODUCT_VERSION}-Windows-Installer.exe"
InstallDir "$PROGRAMFILES\Nootka"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

LangString NelDesc ${LANG_ENGLISH} "Nootka exam level"
LangString NelDesc ${LANG_POLISH} "Poziom egzaminu programu Nootka"
LangString NelDesc ${LANG_CZECH} "Úroveň zkoušky programu Nootka"
LangString NelDesc ${LANG_FRENCH} "Fichier de niveau Nootka"
LangString NelDesc ${LANG_RUSSIAN} "Файл Нутки с уровнем"

LangString NootkaAct ${LANG_ENGLISH} "Open with nootka"
LangString NootkaAct ${LANG_POLISH} "Otwórz w programie Nootka"
LangString NootkaAct ${LANG_CZECH} "Otevřít v Nootka"
LangString NootkaAct ${LANG_FRENCH} "Ouvrir avec Nootka"
LangString NootkaAct ${LANG_RUSSIAN} "Открыть"

LangString NooDesc ${LANG_ENGLISH} "Nootka exam file"
LangString NooDesc ${LANG_POLISH} "Plik egzaminu programu Nootka"
LangString NooDesc ${LANG_CZECH} "Zkušební soubor programu Nootka"
LangString NooDesc ${LANG_FRENCH} "Fichier d'examen Nootka"
LangString NooDesc ${LANG_RUSSIAN} "Файл Нутки с экзаменом"

Section -AssociateMime
;FILE ASSOCIATION

!include fileasoc.nsh
!insertmacro APP_ASSOCIATE "nel" "nootka.level" "$(NelDesc)" "$INSTDIR\picts\nootka-levels.ico"     "$(NootkaAct)" "$INSTDIR\nootka.exe $\"%1$\""
!insertmacro APP_ASSOCIATE "noo" "nootka.exam" "$(NooDesc)" "$INSTDIR\picts\nootka-exam.ico"     "$(NootkaAct)" "$INSTDIR\nootka.exe $\"%1$\""
SectionEnd


Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Section "GrupaGlowna" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "nootka.exe"
  File "nootka-updater.exe"
  CreateDirectory "$SMPROGRAMS\Nootka"
  CreateShortCut "$SMPROGRAMS\Nootka\Nootka.lnk" "$INSTDIR\nootka.exe"
  CreateShortCut "$DESKTOP\Nootka.lnk" "$INSTDIR\nootka.exe"
  File "QtGui4.dll"
  File "QtCore4.dll"
  File "QtNetwork4.dll"
  File "mingwm10.dll"
  File "libgcc_s_dw2-1.dll"
  File "libstdc++-6.dll"
  File "libfftw3f-3.dll"	
  File "libogg.dll"
  File "libvorbis.dll"
  File "libvorbisfile.dll"
;  File "SoundTouch.dll"
  File "gpl"
  File "changes"
  SetOutPath "$INSTDIR\fonts"
  SetOverwrite try
  File "fonts\README"
  File "fonts\nootka.ttf"
  SetOutPath "$INSTDIR\lang"
  File "lang\nootka_pl.qm"
  File "lang\nootka_cs.qm"
  File "lang\nootka_fr.qm"
  File "lang\nootka_ru.qm"
  File "lang\qt_cs.qm"
  File "lang\qt_pl.qm"
  File "lang\qt_ru.qm"
  File "lang\qt_fr.qm"
  SetOutPath "$INSTDIR\picts"
  File "picts\accidSettings.png"
  File "picts\about.png"
  File "picts\author.png"
  File "picts\body.png"
  File "picts\body-bass.png"
  File "picts\body-electro.png"
  File "picts\flags-cs.png"
  File "picts\flags-en.png"
  File "picts\flags-fr.png"
  File "picts\flags-pl.png"
  File "picts\flags-ru.png"
  File "picts\fullscreen.png"
  File "picts\check.png"
  File "picts\chlog.png"
  File "picts\charts.png"
  File "picts\barChart.png"
  File "picts\linearChart.png"
  File "picts\zoom-in.png"
  File "picts\zoom-out.png"
  File "picts\expertCorner.png"
  File "picts\fingbg.png"
  File "picts\fingbg-el.png"
  File "picts\global.png"
  File "picts\guitarSettings.png"
  File "picts\help.png"
  File "picts\levelCreator.png"
  File "picts\levelsSettings.png"
  File "picts\license.png"
  File "picts\logo.png"
  File "picts\nameSettings.png"
  File "picts\nextQuest.png"
  File "picts\nootka-exam.ico"
  File "picts\nootka-exam.png"
  File "picts\nootka-icon.rc"
  File "picts\nootka-levels.ico"
  File "picts\nootka.ico"
  File "picts\nootka.png"
  File "picts\nootka.rc"
  File "picts\notSaved.png"
  File "picts\prevQuest.png"
  File "picts\questionsSettings.png"
  File "picts\rangeSettings.png"
  File "picts\repeatSound.png"
  File "picts\scoreSettings.png"
  File "picts\scr.png"
  File "picts\soundSettings.png"
  File "picts\startExam.png"
  File "picts\practice.png"
  File "picts\stopExam.png"
  File "picts\support.png"
  File "picts\pickup.png"
  File "picts\systemsettings.png"
  SetOutPath "$INSTDIR\sounds"
  File "sounds\classical-guitar.ogg"
  File "sounds\electric-guitar.ogg"
  File "sounds\bass-guitar.ogg"
SectionEnd

Section -AdditionalIcons
  SetOutPath $INSTDIR
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\Nootka\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\Nootka\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\nootka.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\nootka.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


;Function un.onUninstSuccess
;  HideWindow
;  MessageBox MB_ICONINFORMATION|MB_OK "Program $(^Name) zostaÂł pomyÂślnie usuniĂŞty."
;FunctionEnd

   LangString UninstallMess ${LANG_ENGLISH} "Do You realy want to remove $(^Name) and all its components?"
   LangString UninstallMess ${LANG_POLISH} "Czy rzeczywiście chcesz usunąć Nootkę i jej składniki?"
   LangString UninstallMess ${LANG_CZECH} "Opravdu chcete odstranit Nootku a vsechny její soueásti?"
   LangString UninstallMess ${LANG_FRENCH} "Voulez-vous vraiment supprimer Nootka et tous ses composants?"
   LangString UninstallMess ${LANG_RUSSIAN} "Вы действительно хотите удалить Нутку и все её компоненты?"

Function un.onInit
!insertmacro MUI_UNGETLANGUAGE
   MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "$(UninstallMess)" IDYES +2
   Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\sounds\classical-guitar.ogg"
  Delete "$INSTDIR\sounds\electric-guitar.ogg"
  Delete "$INSTDIR\sounds\bass-guitar.ogg"
  Delete "$INSTDIR\picts\systemsettings.png"
  Delete "$INSTDIR\picts\support.png"
  Delete "$INSTDIR\picts\stopExam.png"
  Delete "$INSTDIR\picts\startExam.png"
  Delete "$INSTDIR\picts\practice.png"
  Delete "$INSTDIR\picts\soundSettings.png"
  Delete "$INSTDIR\picts\scr.png"
  Delete "$INSTDIR\picts\scoreSettings.png"
  Delete "$INSTDIR\picts\repeatSound.png"
  Delete "$INSTDIR\picts\rangeSettings.png"
  Delete "$INSTDIR\picts\questionsSettings.png"
  Delete "$INSTDIR\picts\prevQuest.png"
  Delete "$INSTDIR\picts\notSaved.png"
  Delete "$INSTDIR\picts\nootka.rc"
  Delete "$INSTDIR\picts\nootka.png"
  Delete "$INSTDIR\picts\nootka.ico"
  Delete "$INSTDIR\picts\nootka-levels.ico"
  Delete "$INSTDIR\picts\nootka-icon.rc"
  Delete "$INSTDIR\picts\nootka-exam.png"
  Delete "$INSTDIR\picts\nootka-exam.ico"
  Delete "$INSTDIR\picts\nextQuest.png"
  Delete "$INSTDIR\picts\nameSettings.png"
  Delete "$INSTDIR\picts\logo.png"
  Delete "$INSTDIR\picts\license.png"
  Delete "$INSTDIR\picts\levelsSettings.png"
  Delete "$INSTDIR\picts\levelCreator.png"
  Delete "$INSTDIR\picts\help.png"
  Delete "$INSTDIR\picts\guitarSettings.png"
  Delete "$INSTDIR\picts\global.png"
  Delete "$INSTDIR\picts\fingbg.png"
  Delete "$INSTDIR\picts\fingbg-el.png"
  Delete "$INSTDIR\picts\expertCorner.png"
  Delete "$INSTDIR\picts\check.png"
  Delete "$INSTDIR\picts\chlog.png"
  Delete "$INSTDIR\picts\charts.png"
  Delete "$INSTDIR\picts\barChart.png"
  Delete "$INSTDIR\picts\linearChart.png"
  Delete "$INSTDIR\picts\zoom-in.png"
  Delete "$INSTDIR\picts\zoom-out.png"
  Delete "$INSTDIR\picts\flags-cs.png"
  Delete "$INSTDIR\picts\flags-en.png"
  Delete "$INSTDIR\picts\flags-fr.png"
  Delete "$INSTDIR\picts\flags-pl.png"
  Delete "$INSTDIR\picts\flags-ru.png"
  Delete "$INSTDIR\picts\fullscreen.png"
  Delete "$INSTDIR\picts\pickup.png"
  Delete "$INSTDIR\picts\body.png"
  Delete "$INSTDIR\picts\body-bass.png"
  Delete "$INSTDIR\picts\body-electro.png"
  Delete "$INSTDIR\picts\author.png"
  Delete "$INSTDIR\picts\about.png"
  Delete "$INSTDIR\picts\accidSettings.png"
  Delete "$INSTDIR\lang\nootka_pl.qm"
  Delete "$INSTDIR\lang\nootka_cs.qm"
  Delete "$INSTDIR\lang\nootka_fr.qm"
  Delete "$INSTDIR\lang\nootka_ru.qm"
  Delete "$INSTDIR\lang\qt_pl.qm"
  Delete "$INSTDIR\lang\qt_cs.qm"
  Delete "$INSTDIR\lang\qt_fr.qm"
  Delete "$INSTDIR\lang\qt_ru.qm"
  Delete "$INSTDIR\fonts\nootka.ttf"
  Delete "$INSTDIR\fonts\README"
;  Delete "$INSTDIR\SoundTouch.dll"
  Delete "$INSTDIR\libgcc_s_dw2-1.dll"
  Delete "$INSTDIR\libogg.dll"
  Delete "$INSTDIR\libvorbis.dll"
  Delete "$INSTDIR\libvorbisfile.dll"
  Delete "$INSTDIR\libfftw3f-3.dll"
  Delete "$INSTDIR\libstdc++-6.dll"
  Delete "$INSTDIR\mingwm10.dll"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtNetwork4.dll"
  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\nootka.exe"
  Delete "$INSTDIR\nootka-updater.exe"
  Delete "$INSTDIR\gpl"
  Delete "$INSTDIR\changes"

  Delete "$SMPROGRAMS\Nootka\Uninstall.lnk"
  Delete "$SMPROGRAMS\Nootka\Website.lnk"
  Delete "$DESKTOP\Nootka.lnk"
  Delete "$SMPROGRAMS\Nootka\Nootka.lnk"

  RMDir "$SMPROGRAMS\Nootka"
  RMDir "$INSTDIR\sounds"
  RMDir "$INSTDIR\picts"
  RMDir "$INSTDIR\lang"
  RMDir "$INSTDIR\fonts"
  RMDir "$INSTDIR"
  
  !insertmacro APP_UNASSOCIATE "nel" "nootka.level"
  !insertmacro APP_UNASSOCIATE "noo" "nootka.exam"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd