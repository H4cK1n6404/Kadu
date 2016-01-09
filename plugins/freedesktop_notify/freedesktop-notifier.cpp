/*
 * Copyright 2009 Jacek Jabłoński
 * %kadu copyright begin%
 * Copyright 2011 Piotr Galiszewski (piotr.galiszewski@kadu.im)
 * Copyright 2012 Wojciech Treter (juzefwt@gmail.com)
 * Copyright 2011, 2012, 2013 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2011, 2012, 2013, 2014, 2015 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include <QtCore/QFileInfo>
#include <QtCore/QMetaMethod>
#include <QtCore/QTimer>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusServiceWatcher>

#include "configuration/configuration.h"
#include "configuration/deprecated-configuration-api.h"
#include "core/core.h"
#include "core/core.h"
#include "dom/dom-processor-service.h"
#include "icons/kadu-icon.h"
#include "misc/paths-provider.h"
#include "notification/notification/aggregate-notification.h"
#include "notification/notification-callback-repository.h"
#include "notification/notification-callback.h"
#include "notification/notification/notification.h"
#include "notification/notification-event.h"
#include "notification/notification-event-repository.h"
#include "url-handlers/url-handler-manager.h"

#include "freedesktop-notifier.h"

FreedesktopNotifier::FreedesktopNotifier(QObject *parent) :
		QObject{parent},
		Notifier("FreedesktopNotifier", QT_TRANSLATE_NOOP("@default", "System notifications"), KaduIcon("kadu_icons/notify-hints")),
		KdePlasmaNotifications(true), IsXCanonicalAppendSupported(false), ServerSupportsActions(true), ServerSupportsBody(true),
		ServerSupportsHyperlinks(true), ServerSupportsMarkup(true), ServerCapabilitiesRequireChecking(true)
{
	StripBr.setPattern(QLatin1String("<br ?/?>"));
	StripHtml.setPattern(QLatin1String("<[^>]*>"));
	// this is meant to catch all HTML tags except <b>, <i>, <u>
	StripUnsupportedHtml.setPattern(QLatin1String("<(/?[^/<>][^<>]+|//[^>]*|/?[^biu])>"));

	DesktopEntry = QFileInfo(Core::instance()->pathsProvider()->desktopFilePath()).baseName();
	NotificationsInterface = new QDBusInterface("org.freedesktop.Notifications",
			"/org/freedesktop/Notifications", "org.freedesktop.Notifications");

	QDBusServiceWatcher *watcher = new QDBusServiceWatcher(this);
	watcher->setConnection(QDBusConnection::sessionBus());
	watcher->setWatchMode(QDBusServiceWatcher::WatchForOwnerChange);
	watcher->addWatchedService("org.freedesktop.Notifications");

	connect(watcher, SIGNAL(serviceOwnerChanged(QString,QString,QString)),
			SLOT(slotServiceOwnerChanged(QString,QString,QString)));

	NotificationsInterface->connection().connect(NotificationsInterface->service(), NotificationsInterface->path(), NotificationsInterface->interface(),
			"ActionInvoked", this, SLOT(actionInvoked(unsigned int, QString)));
	NotificationsInterface->connection().connect(NotificationsInterface->service(), NotificationsInterface->path(), NotificationsInterface->interface(),
			"NotificationClosed", this, SLOT(notificationClosed(unsigned int, unsigned int)));

	import_0_9_0_Configuration();
	createDefaultConfiguration();

	configurationUpdated();
}

FreedesktopNotifier::~FreedesktopNotifier()
{
	delete NotificationsInterface;
	NotificationsInterface = 0;
}

void FreedesktopNotifier::setDomProcessorService(DomProcessorService *domProcessorService)
{
	m_domProcessorService = domProcessorService;
}

void FreedesktopNotifier::checkServerCapabilities()
{
	if (!ServerCapabilitiesRequireChecking)
		return;

	QDBusMessage replyMsg = NotificationsInterface->call(QDBus::Block, "GetServerInformation");
	if (replyMsg.type() != QDBusMessage::ReplyMessage)
		KdePlasmaNotifications = false;
	else
		KdePlasmaNotifications = replyMsg.arguments().at(0).toString().contains("Plasma") && replyMsg.arguments().at(1).toString().contains("KDE");

	replyMsg = NotificationsInterface->call(QDBus::Block, "GetCapabilities");
	if (replyMsg.type() != QDBusMessage::ReplyMessage)
	{
		IsXCanonicalAppendSupported = false;
		ServerSupportsActions = false;
		ServerSupportsBody = false;
		ServerSupportsHyperlinks = false;
		ServerSupportsMarkup = false;
	}
	else
	{
		QStringList capabilities = replyMsg.arguments().at(0).toStringList();

		IsXCanonicalAppendSupported = capabilities.contains("x-canonical-append");
		ServerSupportsActions = capabilities.contains("actions");
		ServerSupportsBody = capabilities.contains("body");
		ServerSupportsHyperlinks = capabilities.contains("body-hyperlinks");
		ServerSupportsMarkup = capabilities.contains("body-markup");

		ServerCapabilitiesRequireChecking = false;
	}
}

void FreedesktopNotifier::notify(Notification *notification)
{
	checkServerCapabilities();

	bool useKdeStyle = KdePlasmaNotifications && ServerSupportsBody && ServerSupportsMarkup;

	QList<QVariant> args;
	args.append("Kadu");

	unsigned int replacedNotificationId = 0U;
	unsigned int notificationUid = NotificationMap.key(notification);

	if (notificationUid)
	{
		notificationClosed(notification);
		replacedNotificationId = notificationUid;
	}

	args.append(replacedNotificationId);

	KaduIcon icon(notification->icon());
	if (icon.isNull())
	{
		icon.setPath("kadu_icons/section-kadu");
		icon.setSize("32x32");
	}
	else
		icon.setSize("64x64");
	args.append(icon.fullPath());

	QString summary;
	if (useKdeStyle)
		summary = "Kadu";
	else
	{
		summary = notification->text();
		summary.replace(StripBr, QLatin1String(" "));
		summary.remove(StripHtml);
	}

	args.append(summary);

	bool typeNewMessage = (notification->type() == "NewMessage" || notification->type() == "NewChat");
	QString body;
	if (ServerSupportsBody)
	{
		if (!typeNewMessage || ShowContentMessage)
		{
			body = !notification->details().isEmpty()
					? notification->details().last()
					: QString();
			body.replace(StripBr, QLatin1String("\n"));
			if (ServerSupportsMarkup)
				body.remove(StripUnsupportedHtml);
			else
				body.remove(StripHtml);

			if (body.length() > CiteSign)
				body = body.left(CiteSign) + QLatin1String("...");
		}

		if (useKdeStyle)
		{
			if (body.isEmpty())
				body = notification->text();
			else
			{
				body.prepend(notification->text() + "\n<small>");
				body.append("</small>");
			}

			if (body.length() > CiteSign)
				body = body.left(CiteSign) + QLatin1String("...");
		}

		if (ServerSupportsHyperlinks && !body.isEmpty())
			body = m_domProcessorService->process(body);
	}

	args.append(body);

	QStringList actions;
	if (ServerSupportsActions)
	{
		Notification *firstNotification = notification;

		AggregateNotification *aggregateNotification = qobject_cast<AggregateNotification *>(notification);
		if (aggregateNotification)
		{
			firstNotification = aggregateNotification->notifications().first();
		}

		for (auto &&callbackName : firstNotification->getCallbacks())
		{
			auto callback = Core::instance()->notificationCallbackRepository()->callback(callbackName);
			actions << callbackName;
			actions << callback.title();
		}
	}
	args.append(actions);

	QVariantMap hints;
	hints.insert("category", typeNewMessage ? "im.received" : "im");
	hints.insert("desktop-entry", DesktopEntry);
	if (IsXCanonicalAppendSupported && !useKdeStyle)
		hints.insert("x-canonical-append", "allowed");
	args.append(hints);

	// -1 is server default
	args.append(CustomTimeout ? Timeout * 1000 : -1);

	QDBusReply<unsigned int> reply = NotificationsInterface->callWithArgumentList(QDBus::Block, "Notify", args);
	if (reply.isValid())
	{
		notification->acquire(this); // do not remove now

		connect(notification, SIGNAL(closed(Notification*)), this, SLOT(notificationClosed(Notification*)));

		NotificationMap.insert(reply.value(), notification);
	}
}

void FreedesktopNotifier::notificationClosed(Notification *notification)
{
	QMap<unsigned int, Notification *>::iterator i = NotificationMap.begin();
	while (i != NotificationMap.end())
	{
		if (i.value() == notification)
		{
			QList<QVariant> args;
			args.append(i.key());
			NotificationsInterface->callWithArgumentList(QDBus::Block, "CloseNotification", args);

			NotificationMap.erase(i);

			return;
		}

		++i;
	}
}

void FreedesktopNotifier::notificationClosed(unsigned int id, unsigned int reason)
{
	Q_UNUSED(reason);

	if (!NotificationMap.contains(id))
		return;

	Notification *notification = NotificationMap.take(id);
	disconnect(notification, SIGNAL(closed(Notification*)), this, SLOT(notificationClosed(Notification*)));
	notification->release(this);
}

void FreedesktopNotifier::slotServiceOwnerChanged(const QString &serviceName, const QString &oldOwner, const QString &newOwner)
{
	Q_UNUSED(serviceName)
	Q_UNUSED(oldOwner)
	Q_UNUSED(newOwner)

	foreach (Notification *notification, NotificationMap)
	{
		disconnect(notification, SIGNAL(closed(Notification*)), this, SLOT(notificationClosed(Notification*)));
		notification->release(this);
	}

	NotificationMap.clear();

	ServerCapabilitiesRequireChecking = true;
}

void FreedesktopNotifier::actionInvoked(unsigned int id, QString callbackName)
{
	if (!NotificationMap.contains(id))
		return;

	auto notification = NotificationMap.value(id);
	if (!notification)
		return;

	auto callbackNotifiation = notification;
	if (qobject_cast<AggregateNotification *>(callbackNotifiation))
		callbackNotifiation = qobject_cast<AggregateNotification *>(callbackNotifiation)->notifications()[0];

	auto callback = Core::instance()->notificationCallbackRepository()->callback(callbackName);
	callback.call(callbackNotifiation);
	notification->close();

	QList<QVariant> args;
	args.append(id);
	NotificationsInterface->callWithArgumentList(QDBus::Block, "CloseNotification", args);
}

void FreedesktopNotifier::configurationUpdated()
{
	CustomTimeout = Core::instance()->configuration()->deprecatedApi()->readBoolEntry("FreedesktopNotifier", "CustomTimeout");
	Timeout = Core::instance()->configuration()->deprecatedApi()->readNumEntry("FreedesktopNotifier", "Timeout");
	ShowContentMessage = Core::instance()->configuration()->deprecatedApi()->readBoolEntry("FreedesktopNotifier", "ShowContentMessage");
	CiteSign = Core::instance()->configuration()->deprecatedApi()->readNumEntry("FreedesktopNotifier", "CiteSign");
}

void FreedesktopNotifier::import_0_9_0_Configuration()
{
	Core::instance()->configuration()->deprecatedApi()->addVariable("FreedesktopNotifier", "Timeout", Core::instance()->configuration()->deprecatedApi()->readEntry("KDENotify", "Timeout"));
	Core::instance()->configuration()->deprecatedApi()->addVariable("FreedesktopNotifier", "ShowContentMessage", Core::instance()->configuration()->deprecatedApi()->readEntry("KDENotify", "ShowContentMessage"));
	Core::instance()->configuration()->deprecatedApi()->addVariable("FreedesktopNotifier", "CiteSign", Core::instance()->configuration()->deprecatedApi()->readEntry("KDENotify", "CiteSign"));
	if (!Core::instance()->configuration()->deprecatedApi()->readEntry("KDENotify", "Timeout").isEmpty() || !Core::instance()->configuration()->deprecatedApi()->readEntry("FreedesktopNotifier", "Timeout").isEmpty())
		Core::instance()->configuration()->deprecatedApi()->addVariable("FreedesktopNotifier", "CustomTimeout", true);

	for (auto &&event : Core::instance()->notificationEventRepository()->notificationEvents())
		Core::instance()->configuration()->deprecatedApi()->addVariable("Notify", event.name() + "_FreedesktopNotifier", Core::instance()->configuration()->deprecatedApi()->readEntry("Notify", event.name() + "_KNotify"));
}

void FreedesktopNotifier::createDefaultConfiguration()
{
	Core::instance()->configuration()->deprecatedApi()->addVariable("Notify", "ConnectionError_FreedesktopNotifier", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Notify", "NewChat_FreedesktopNotifier", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Notify", "NewMessage_FreedesktopNotifier", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Notify", "StatusChanged_FreedesktopNotifier", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Notify", "StatusChanged/ToFreeForChat_FreedesktopNotifier", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Notify", "StatusChanged/ToOnline_FreedesktopNotifier", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Notify", "StatusChanged/ToAway_FreedesktopNotifier", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Notify", "StatusChanged/ToNotAvailable_FreedesktopNotifier", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Notify", "StatusChanged/ToDoNotDisturb_FreedesktopNotifier", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Notify", "StatusChanged/ToOffline_FreedesktopNotifier", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Notify", "FileTransfer_FreedesktopNotifier", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Notify", "FileTransfer/IncomingFile_FreedesktopNotifier", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Notify", "FileTransfer/Finished_FreedesktopNotifier", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Notify", "multilogon_FreedesktopNotifier", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Notify", "multilogon/sessionConnected_FreedesktopNotifier", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Notify", "multilogon/sessionDisconnected_FreedesktopNotifier", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Notify", "Roster/ImportFailed_UseCustomSettings", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Notify", "Roster/ImportFailed_FreedesktopNotifier", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Notify", "Roster/ExportFailed_UseCustomSettings", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Notify", "Roster/ExportFailed_FreedesktopNotifier", true);

	Core::instance()->configuration()->deprecatedApi()->addVariable("FreedesktopNotifier", "CustomTimeout", false);
	Core::instance()->configuration()->deprecatedApi()->addVariable("FreedesktopNotifier", "Timeout", 10);
	Core::instance()->configuration()->deprecatedApi()->addVariable("FreedesktopNotifier", "ShowContentMessage", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("FreedesktopNotifier", "CiteSign", 100);
}

#include "moc_freedesktop-notifier.cpp"