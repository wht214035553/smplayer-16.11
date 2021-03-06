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

#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QString>
#include <QVariant>
#include <QList>

#ifdef Q_OS_WIN
#define USE_DSOUND_DEVICES 1
#else
#define USE_ALSA_DEVICES 0
#define USE_PULSEAUDIO_DEVICES 1
#define USE_XV_ADAPTORS 1
#endif

#ifndef Q_OS_WIN
#define CACHE_DEVICE_INFO
#endif

class QSettings;

class DeviceData {

public:
	DeviceData() {};
	DeviceData(QVariant ID, QString desc) { _id = ID; _desc = desc; };
	~DeviceData() {};

	void setID(QVariant ID) { _id = ID; };
	void setDesc(QString desc) { _desc = desc; };

	QVariant ID() const { return _id; };
	QString desc() const { return _desc; };

private:
	QVariant _id;
	QString _desc;
};


typedef QList<DeviceData> DeviceList;


class DeviceInfo {

public:
#ifdef Q_OS_WIN
	static DeviceList dsoundDevices();
	static DeviceList displayDevices();
#else
	#if USE_PULSEAUDIO_DEVICES
	static DeviceList paDevices();
	#endif
	#if USE_ALSA_DEVICES
	static DeviceList alsaDevices();
	#endif
	#if USE_XV_ADAPTORS
	static DeviceList xvAdaptors();
	#endif
#endif

protected:
#ifdef CACHE_DEVICE_INFO
	static void saveList(QSettings * set, const QString & section_name, const DeviceList & list);
	static DeviceList loadList(QSettings * set, const QString & section_name);
#endif

#ifdef Q_OS_WIN
	enum DeviceType { Sound = 0, Display = 1 };

	static DeviceList retrieveDevices(DeviceType type);
#endif
};

#endif

