/*
 * Copyright 2009 Jacek Jabłoński
 * %kadu copyright begin%
 * Copyright 2011, 2012, 2013 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2011, 2012, 2013, 2014 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#pragma once

#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QQueue>
#include <QtCore/QRegExp>

#include "configuration/configuration-aware-object.h"
#include "notification/notifier.h"

class QDBusInterface;

class DomProcessorService;
class Notification;

class FreedesktopNotifier : public QObject, public Notifier, public ConfigurationAwareObject
{
	Q_OBJECT

public:
	Q_INVOKABLE explicit FreedesktopNotifier(QObject *parent = nullptr);
	virtual ~FreedesktopNotifier();

	virtual NotifierConfigurationWidget *createConfigurationWidget(QWidget *parent = nullptr) { Q_UNUSED(parent); return 0; }
	virtual void notify(Notification *notification);

protected:
	void configurationUpdated();

private:
	QPointer<DomProcessorService> m_domProcessorService;

	QDBusInterface *NotificationsInterface;
	QRegExp StripBr;
	QRegExp StripHtml;
	QRegExp StripUnsupportedHtml;
	QMap<unsigned int, Notification *> NotificationMap;

	bool CustomTimeout;
	int Timeout;
	bool ShowContentMessage;
	int CiteSign;

	bool KdePlasmaNotifications;
	bool IsXCanonicalAppendSupported;
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
	INJEQT_SET void setDomProcessorService(DomProcessorService *domProcessorService);

	void actionInvoked(unsigned int id, QString action);

	void notificationClosed(Notification *notification);
	void notificationClosed(unsigned int id, unsigned int reason);

	void slotServiceOwnerChanged(const QString &serviceName, const QString &oldOwner, const QString &newOwner);

};