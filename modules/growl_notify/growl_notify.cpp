/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/* This program uses the GrowlNotifier, located in growlnotifier.h and
 * growlnotifier.cpp file, which are the part of PSI Jabber Client.
 */

#include "../notify/notify.h"
#include "../notify/notification.h"

#include <QtGui/QTextDocument>

#include "config_file.h"
#include "debug.h"
#include "icons_manager.h"
#include "message_box.h"
#include "userlist.h"
#include "chat_manager.h"
#include "kadu_parser.h"

#include "growl_notify.h"
#include "growlnotifier.h"
#include "growl_notify_configuration_widget.h"

/**
 * @ingroup growl_notify
 * @{
 */
extern "C" KADU_EXPORT int growl_notify_init(bool firstLoad)
{
#ifndef Q_OS_MAC
	/* growl is only supported on MacOSX */
	return 1;
#endif
	growl_notify = new GrowlNotify(0, "growl_notify");
	return 0;
}

extern "C" KADU_EXPORT void growl_notify_close()
{
	delete growl_notify;
	growl_notify = 0;
}

GrowlNotify::GrowlNotify(QObject *parent, const char *name) : Notifier(parent, name)
{
	kdebugf();

	createDefaultConfiguration();
	notification_manager->registerNotifier(QT_TRANSLATE_NOOP("@default", "Growl"), this);

	// Initialize GrowlNotifier
	QStringList notifications;
	notifications << "Kadu Notification";
	growlNotifier = new GrowlNotifier(notifications, notifications, "Kadu");

	kdebugf2();
}

GrowlNotify::~GrowlNotify()
{
	kdebugf();
	notification_manager->unregisterNotifier("Growl");
	kdebugf2();
}

QString GrowlNotify::toPlainText(const QString &text)
{
	QTextDocument doc;
	doc.setHtml(text);
	return doc.toPlainText();
}

QString GrowlNotify::parseText(const QString &text, Notification *notification, const QString &def)
{
	UserListElement ule;
	QString ret;

	if (notification->userListElements().count())
		ule = notification->userListElements()[0];

	if (!text.isEmpty())
	{
		ret = KaduParser::parse(text, ule, notification);
		ret = ret.replace("%&m", notification->text());
		ret = ret.replace("%&t", notification->title());
		ret = ret.replace("%&d", notification->details());
	}
	else
		ret = def;

	return toPlainText(ret);
}


void GrowlNotify::notify(Notification *notification)
{
	kdebugf();

	QString title = config_file.readEntry("GrowlNotify", QString("Event_") + notification->type() + "_title");
	QString syntax = config_file.readEntry("GrowlNotify", QString("Event_") + notification->type() + "_syntax");

	notification->acquire();
	senders = UserListElements(notification->userListElements());

	growlNotifier->notify("Kadu Notification", 
		parseText(title, notification, notification->text()),
		parseText(syntax, notification, notification->details()),
		icons_manager->loadPixmap(notification->icon()),
		false, this, SLOT(notification_clicked()));

	notification->release();

	kdebugf2();
}

void GrowlNotify::notification_clicked()
{
	if (!senders.isEmpty())
		chat_manager->openPendingMsgs(senders, true);
}

void GrowlNotify::createDefaultConfiguration()
{
	config_file.addVariable("Notify", "FileTransfer/IncomingFile_Window", true);
}

NotifierConfigurationWidget *GrowlNotify::createConfigurationWidget(QWidget *parent, char *name)
{
	configurationWidget = new GrowlNotifyConfigurationWidget(parent, name);
	return configurationWidget;
}

GrowlNotify *growl_notify = 0;

/** @} */

