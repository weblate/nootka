
## HOW TO TRANSLATE NOOTKA
---------------------------


#### This process is very easy and does not require any programming skills, only some time and a bit of patience.
---------------------------


#### Translation with Qt Linguist or lokalize (this is recommended way)
  1. Install Qt Linguist:  
    Linux users will prefer using package manager of their distribution. Linguist will be there.  
    For Mac and Windows users:
      - form https://www.qt.io/download-open-source/ download online installer,
        launch it and follow its instructions - select the latest version of Qt for Desktop.

     or use other software for translators that supports *.ts format,  
     i.e. [lokalize](https://apps.kde.org/en/lokalize).
      
  2. Download [Nootka language file: nootka_xx.ts](https://www.opencode.net/seelook/nootka/raw/master/lang/nootka_xx.ts?inline=false)  
     and change that xx in file name to your language code to get file name like f.e.: nootka_en.ts

  3. Open this file by Qt Linguist (or any other app) and translate - this is very easy.

  4. In every moment you can generate a "language package" form menu File->Release (file with *.qm extension)
     and put that file in Nootka installation directory into the lang folder 
     i.e.: path_to_nootka/lang/nootka_en.qm
     and run Nootka to see effects of the translation
     To test translation under Android version:
       - create Nootka folder on starage where Nootka is installed (device built-in or memory card)
       - then copy *.qm file there
       - be sure, Nootka has write permissions set

---------------------------
#### Translation online trough LaunchPad
     A disadvantage of this method is that you can not easily generate translation package (*.qm)
     to check your translation in action.
  1. You will need to have account on https://launchpad.net/
  2. Then visit a site: https://translations.launchpad.net/nootka
  3. Start translation or continue existing one.
  4. To generate Nootka-loadable file and launch Nootka with it you have to:
     - download *.po file from site
     - convert it to *.qm (it is necessary to have Qt installed.  
       Command line lconvert program is shipped with it)  
       `lconvert -i your_file.po -o nootka_languageCode.qm`
     - and put that file in Nootka installation directory into the lang folder

---------------------------
#### A Few Hints
  - (Only in Qt Linguist) To approve translated entry you can use \[CTRL+Enter\] - it will be automatically marked as finished.
  - Do not remove any special characters like \n %1 %n or HTML tags like <br> and <whatever>.
  - To copy original entry use [CTRL+b] and then just translate.  
    Obviously you can move this mischievous characters and tags to make whole translated sentence look in proper way in your language
   - Often use \[CTRL+s\] to save your work
   - PLEASE BE RESPONSIBLE !!! USE KIND WORDS AND OFFICIAL MUSICAL TERMS.  
     DO NOT USE SOME SLANG WORDS EVEN MUSICAL. Nootka is used by children and pupils


---------------------------
When translation will be ready or you give up :-(, send me what you have:  
    seelook@gmail.com  
    How to give some info about translator (you) you will find during translation.

    Have a fun !!!


