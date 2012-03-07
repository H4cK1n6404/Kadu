/*
 * Copyright 2009 Jacek Jabłoński
 * %kadu copyright begin%
 * Copyright 2008, 2009, 2010, 2011 Piotr Galiszewski (piotr.galiszewski@kadu.im)
 * Copyright 2008 Michał Podsiadlik (michal@kadu.net)
 * Copyright 2007, 2008, 2009, 2010, 2011 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * Copyright 2011 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2007, 2008 Dawid Stawiarski (neeo@kadu.net)
 * %kadu copyright end%
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FREEDESKTOP_NOTIFY_H
#define FREEDESKTOP_NOTIFY_H

#include <QtCore/QMap>
#include <QtCore/QQueue>
#include <QtCore/QRegExp>

#include "configuration/configuration-aware-object.h"
#include "notify/notifier.h"

class QDBusInterface;

class Notification;

class FreedesktopNotify : public Notifier, public ConfigurationAwareObject
{
	Q_OBJECT

	static FreedesktopNotify *Instance;

	explicit FreedesktopNotify();
	virtual ~FreedesktopNotify();

	QDBusInterface *KNotify;
	QRegExp StripBr;
	QRegExp StripHtml;
	QRegExp StripUnsupportedHtml;
	QMap<unsigned int, Notification *> NotificationMap;
	QQueue<unsigned int> IdQueue;

	bool CustomTimeout;
	int Timeout;
	bool ShowContentMessage;
	int CiteSign;

	bool KdePlasmaNotifications;
	bool IsXCanonicalAppendSupported;
	bool UseFreedesktopStandard;
	bool ServerSupportsActions;
	bool ServerSupportsBody;
	bool ServerSupportsHyperlinks;
	bool ServerSupportsMarkup;
	bool ServerCapabilitiesRequireChecking;

	QString DesktopEntry;

	void import_0_9_0_Configuration();
	void createDefaultConfiguration();

	void checkServerCapabilities();

private slots:
	void actionInvoked(unsigned int id, QString action);
	void deleteMapItem();

	void notificationClosed(Notification *notification);

	void slotServiceOwnerChanged(const QString &serviceName, const QString &oldOwner, const QString &newOwner);

protected:
	void configurationUpdated();

public:
	static void createInstance();
	static void destroyInstance();
	static FreedesktopNotify * instance();

	virtual CallbackCapacity callbackCapacity();

	virtual NotifierConfigurationWidget *createConfigurationWidget(QWidget *parent = 0) { Q_UNUSED(parent); return 0; }
	virtual void notify(Notification *notification);

};

#endif // FREEDESKTOP_NOTIFY_H
