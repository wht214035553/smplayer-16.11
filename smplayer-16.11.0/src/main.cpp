/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2016 Ricardo Villalba <rvm@users.sourceforge.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "myapplication.h"
#include "smplayer.h"

#include <QDir>

#ifdef HDPI_SUPPORT
#include "paths.h"
#include "hdpisupport.h"
#endif

int main( int argc, char ** argv )
{
#ifdef HDPI_SUPPORT
	HDPISupport * hdpi = new HDPISupport(Paths::configPath());
#endif

	MyApplication a( "smplayer", argc, argv );
	/*
	if (a.isRunning()) { 
		qDebug("Another instance is running. Exiting.");
		return 0;
	}
	*/

    // This property holds whether the application implicitly quits when the
    // last window is closed.
	a.setQuitOnLastWindowClosed(false);
	
#ifdef Q_OS_WIN
	// Change the working directory to the application path
	QDir::setCurrent(a.applicationDirPath());
#endif

#if QT_VERSION >= 0x040400
	// Enable icons in menus
	QCoreApplication::setAttribute(Qt::AA_DontShowIconsInMenus, false);
#endif

	// Sets the config path
	QString config_path;

#ifdef PORTABLE_APP
    // Returns the directory that contains the application executable.
	config_path = a.applicationDirPath();
#else
	// If a smplayer.ini exists in the app path, will use that path
	// for the config file by default
    //  Returns the directory that contains the application executable
	if (QFile::exists( a.applicationDirPath() + "/smplayer.ini" ) ) {
		config_path = a.applicationDirPath();
        // toUtf8 Returns a UTF-8 representation(biao shi) of the string as a QByteArray.
        // data Returns a pointer to the data stored in the byte array
		qDebug("main: using existing %s", QString(config_path + "/smplayer.ini").toUtf8().data());
	}
#endif
    // Returns the list of command-line arguments.
	QStringList args = a.arguments();
    //Returns the index position of the first exact match of rx in the list,
    //searching forward from index position from. Returns -1 if no item matched.
	int pos = args.indexOf("-config-path");
	if ( pos != -1) {
		if (pos+1 < args.count()) {
			pos++;
			config_path = args[pos];
			// Delete from list
			args.removeAt(pos);
			args.removeAt(pos-1);
		} else {
			printf("Error: expected parameter for -config-path\r\n");
			return SMPlayer::ErrorArgument;
		}
	}

	SMPlayer * smplayer = new SMPlayer(config_path);
	SMPlayer::ExitCode c = smplayer->processArgs( args );
	if (c != SMPlayer::NoExit) {
		return c;
	}
	smplayer->start();

	int r = a.exec();

	delete smplayer;

#ifdef HDPI_SUPPORT
	delete hdpi;
#endif

	return r;
}

