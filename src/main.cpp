/***************************************************************************
 *   Copyright (C) 2011-2014 by Tomasz Bojczuk                             *
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
#include <tinitcorelib.h>
#include <QPointer>
#include <QFile>
#include <QSettings>
#include <QApplication>
#include <QDebug>
#include <QTranslator>


void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
  if (type == QtDebugMsg) {
    QFile outFile("nootka-log.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << msg << endl;
  }
}


Tglobals *gl;
bool resetConfig;


int main(int argc, char *argv[])
{    
//   qInstallMessageHandler(myMessageOutput);
	QTranslator qtTranslator;
// 	QTranslator qtbaseTranslator;
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
		gl = new Tglobals();
		gl->path = Tglobals::getInstPath(qApp->applicationDirPath());
		confFile = gl->config->fileName();
		if (!initCoreLibrary())
			return 110;
		prepareTranslations(a, qtTranslator, nooTranslator, nooTranslator); // 2nd argument is unused
		if (!loadNootkaFont(a))
			return 111;
		
// creating main window
    w = new MainWindow();

#if defined (Q_OS_MAC)
	// to allow opening nootka files by clicking them in MacOs finder
    a->installEventFilter(w);
#endif
#if defined (Q_OS_ANDROID)
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
	} while (resetConfig);

	return exitCode;
}
