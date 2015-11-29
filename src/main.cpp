/***************************************************************************
 *   Copyright (C) 2011-2015 by Tomasz Bojczuk                             *
 *   tomaszbojczuk@gmail.com                                               *
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
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/

#include "mainwindow.h"
#if defined (Q_OS_ANDROID)
  #include "ttouchstyle.h"
  // #include <QtWidgets/qstylefactory.h>
#endif
#include <tinitcorelib.h>
#include <tmtr.h>
#include <QtCore/qpointer.h>
#include <QtCore/qfile.h>
#include <QtCore/qsettings.h>
#include <QtWidgets/qapplication.h>
#include <QtCore/qdebug.h>
#include <QtCore/qtranslator.h>
#include <QtCore/qdatetime.h>


/** It allows to grab all debug messages into nootka-log.txt file */
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
  Q_UNUSED(context)
  Q_UNUSED(type)
//   if (type == QtDebugMsg) {
#if defined (Q_OS_ANDROID)
    QFile outFile(qgetenv("EXTERNAL_STORAGE") + QStringLiteral("/nootka-log.txt"));
#else
    QFile outFile(QStringLiteral("nootka-log.txt"));
#endif
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << msg << endl;
//   }
}


Tglobals *gl;
bool resetConfig;


int main(int argc, char *argv[])
{
#if defined (Q_OS_ANDROID)
  {
    QString logFile(qgetenv("EXTERNAL_STORAGE") + QStringLiteral("/nootka-log.txt"));
    if (QFile::exists(logFile))
      QFile::remove(logFile);
  }
  qInstallMessageHandler(myMessageOutput);
  qDebug() << "==== NOOTKA LOG =======\n" << QDateTime::currentDateTime().toString();
#endif
	QTranslator qtTranslator;
	QTranslator nooTranslator;
	QPointer<QApplication> a = 0;
	MainWindow *w = 0;
	int exitCode;
	bool firstTime = true;
	QString confFile;
	resetConfig = false;
	do {		
		if (a) delete a;
		if (resetConfig) { // delete config file - new Nootka instance will start with first run wizard
				QFile f(confFile);
				f.remove();
		}
		resetConfig = false;
		a = new QApplication(argc, argv);
    Tmtr::init(a);
#if defined (Q_OS_ANDROID)
    a->setStyle(new TtouchStyle);
//     a->setStyle(QStyleFactory::create("Fusion"));
#endif
		gl = new Tglobals();
		gl->path = Tglobals::getInstPath(qApp->applicationDirPath());
		confFile = gl->config->fileName();
		if (!initCoreLibrary())
			return 110;
		prepareTranslations(a, qtTranslator, nooTranslator);
		if (!loadNootkaFont(a))
			return 111;
		
// creating main window
    w = new MainWindow();

#if defined (Q_OS_MAC)
	// to allow opening nootka files by clicking them in MacOs finder
    a->installEventFilter(w);
#endif
#if defined (Q_OS_ANDROID)
    qDebug() << "params" << argc;
    w->showFullScreen();
#else
    w->show();
#endif

    if (firstTime && argc > 1)
        w->openFile(QString::fromLocal8Bit(argv[argc - 1]));
		firstTime = false;
		exitCode = a->exec();
		delete w;
    delete gl;
#if defined (Q_OS_ANDROID)
    resetConfig = false; // Close Android app anyway - it doesn't support restarting
#endif
	} while (resetConfig);
  delete a;
	return exitCode;
}
