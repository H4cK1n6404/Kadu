/*
 * %kadu copyright begin%
 * Copyright 2012 Wojciech Treter (juzefwt@gmail.com)
 * Copyright 2011, 2012, 2013, 2014 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2011, 2013, 2014 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include <QtGui/QTextDocument>
#include <QtWidgets/QSystemTrayIcon>

#include "notification/notification-manager.h"
#include "notification/notification/notification.h"

#include "configuration/configuration.h"
#include "configuration/deprecated-configuration-api.h"
#include "core/core.h"
#include "core/core.h"
#include "gui/widgets/chat-widget/chat-widget-manager.h"
#include "gui/windows/message-dialog.h"
#include "parser/parser.h"
#include "debug.h"

#include "plugins/docking/docking-plugin-object.h"
#include "plugins/docking/docking.h"

#include "docking-notifier.h"

/**
 * @ingroup qt4_notify
 * @{
 */

DockingNotifier::DockingNotifier(QObject *parent) :
		QObject{parent},
		Notifier("Tray Icon Balloon", QT_TRANSLATE_NOOP("@default", "Tray Icon Balloon"), KaduIcon("external_modules/qt4notify")),
		configurationWidget{}
{
	createDefaultConfiguration();
}

DockingNotifier::~DockingNotifier()
{
}

void DockingNotifier::setDocking(Docking *docking)
{
	connect(docking, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
}

QString DockingNotifier::toPlainText(const QString &text)
{
	QTextDocument doc;
	doc.setHtml(text);
	return doc.toPlainText();
}

QString DockingNotifier::parseText(const QString &text, Notification *notification, const QString &def)
{
	QString ret;

	chat = notification->data()["chat"].value<Chat>();

	if (!text.isEmpty())
	{
		if (chat)
		{
			Contact contact = *chat.contacts().constBegin();
			ret = Parser::parse(text, Talkable(contact), notification, ParserEscape::HtmlEscape);
		}
		else
			ret = Parser::parse(text, notification, ParserEscape::HtmlEscape);

		ret = ret.replace("%&m", notification->text());
		ret = ret.replace("%&t", notification->title());
		ret = ret.replace("%&d", notification->details().join(QLatin1String("\n")));
	}
	else
		ret = def;


	return toPlainText(ret);
}

void DockingNotifier::notify(Notification *notification)
{
	kdebugf();

	notification->acquire(this);

	unsigned int timeout = Core::instance()->configuration()->deprecatedApi()->readNumEntry("Qt4DockingNotifier", QString("Event_") + notification->key() + "_timeout");
	unsigned int icon = Core::instance()->configuration()->deprecatedApi()->readNumEntry("Qt4DockingNotifier", QString("Event_") + notification->key() + "_icon");
	QString title = Core::instance()->configuration()->deprecatedApi()->readEntry("Qt4DockingNotifier", QString("Event_") + notification->key() + "_title");
	QString syntax = Core::instance()->configuration()->deprecatedApi()->readEntry("Qt4DockingNotifier", QString("Event_") + notification->key() + "_syntax");

	m_docking->showMessage(parseText(title, notification, notification->text()),
		parseText(syntax, notification, notification->details().join(QLatin1String("\n"))),
		(QSystemTrayIcon::MessageIcon)icon, timeout * 1000);

	notification->release(this);

	kdebugf2();
}

void DockingNotifier::messageClicked()
{
	Core::instance()->chatWidgetManager()->openChat(chat, OpenChatActivation::Activate);
}

NotifierConfigurationWidget *DockingNotifier::createConfigurationWidget(QWidget *parent)
{
	configurationWidget = new DockingNotifyConfigurationWidget(parent);
	return configurationWidget;
}

void DockingNotifier::createDefaultConfiguration()
{
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_ConnectionError_timeout", 10);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_ConnectionError_syntax", "%&m");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_ConnectionError_title", "%&t");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_ConnectionError_icon", 3);

	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_NewChat_timeout", 10);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_NewChat_syntax", "%&d");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_NewChat_title", "%&m");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_NewChat_icon", 1);

	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_NewMessage_timeout", 10);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_NewMessage_syntax", "%&d");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_NewMessage_title", "%&m");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_NewMessage_icon", 1);

	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged_timeout", 10);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged_syntax", "%&d");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged_title", "%&m");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChangedt_icon", 0);

	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToFreeForChat_timeout", 10);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToFreeForChat_syntax", "%&d");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToFreeForChat_title", "%&m");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToFreeForChat_icon", 0);

	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToOnline_timeout", 10);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToOnline_syntax", "%&d");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToOnline_title", "%&m");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToOnline_icon", 0);

	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToAway_timeout", 10);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToAway_syntax", "%&d");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToAway_title", "%&m");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToAway_icon", 0);

	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToNotAvailable_timeout", 10);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToNotAvailable_syntax", "%&d");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToNotAvailable_title", "%&m");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToNotAvailable_icon", 0);

	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToDoNotDisturb_timeout", 10);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToDoNotDisturb_syntax", "%&d");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToDoNotDisturb_title", "%&m");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToDoNotDisturb_icon", 0);

	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToOffline_timeout", 10);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToOffline_syntax", "%&d");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToOffline_title", "%&m");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_StatusChanged/ToOffline_icon", 0);

	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_FileTransfer/Finished_timeout", 10);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_FileTransfer/Finished_syntax", "%&m");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_FileTransfer/Finished_title", "%&t");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_FileTransfer/Finished_icon", 2);

	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_FileTransfer/IncomingFile_timeout", 10);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_FileTransfer/IncomingFile_syntax", "%&m");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_FileTransfer/IncomingFile_title", "%&t");
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Event_FileTransfer/IncomingFile_icon", 2);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Qt4DockingNotifier", "Qt4DockingNotificationEventConfiguration_Geometry", "50, 50, 615, 290");
}

/** @} */


#include "moc_docking-notifier.cpp"
