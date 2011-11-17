/*
 * %kadu copyright begin%
 * Copyright 2010 Piotr Dąbrowski (ultr@ultr.pl)
 * Copyright 2010, 2011 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2009 Wojciech Treter (juzefwt@gmail.com)
 * Copyright 2009, 2010, 2011 Piotr Galiszewski (piotr.galiszewski@kadu.im)
 * Copyright 2009, 2010, 2011 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * Copyright 2009 Michał Podsiadlik (michal@kadu.net)
 * Copyright 2009 Bartłomiej Zimoń (uzi18@o2.pl)
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

#include <QtGui/QApplication>

#include "chat/aggregate-chat-manager.h"
#include "chat/message/message-manager.h"
#include "chat/message/message-render-info.h"
#include "chat/message/pending-messages-manager.h"
#include "chat/chat-manager.h"
#include "configuration/configuration-file.h"
#include "contacts/contact.h"
#include "contacts/contact-set.h"
#include "core/core.h"
#include "gui/widgets/chat-widget.h"
#include "gui/widgets/chat-widget-actions.h"
#include "gui/widgets/chat-widget-container.h"
#include "gui/windows/chat-window.h"
#include "gui/windows/kadu-window.h"
#include "icons/icons-manager.h"
#include "protocols/protocol-factory.h"
#include "activate.h"

#include "chat-widget-manager.h"

ChatWidgetManager * ChatWidgetManager::Instance = 0;

ChatWidgetManager * ChatWidgetManager::instance()
{
	if (0 == Instance)
	{
		Instance = new ChatWidgetManager();
		// Load configuration in constructor creates loop because Instance == 0
		Instance->ensureLoaded();
	}

	return Instance;
}

ChatWidgetManager::ChatWidgetManager()
{
	setState(StateNotLoaded);

	MessageRenderInfo::registerParserTags();

	connect(MessageManager::instance(), SIGNAL(messageReceived(const Message &)),
			this, SLOT(messageReceived(const Message &)));
	connect(MessageManager::instance(), SIGNAL(messageSent(const Message &)),
			this, SLOT(messageSent(const Message &)));

	Actions = new ChatWidgetActions(this);

	configurationUpdated();
}


ChatWidgetManager::~ChatWidgetManager()
{
	MessageRenderInfo::unregisterParserTags();

	disconnect(MessageManager::instance(), SIGNAL(messageReceived(const Message &)),
			this, SLOT(messageReceived(const Message &)));

	closeAllWindows();
}

StorableObject * ChatWidgetManager::storageParent()
{
	return 0;
}

QString ChatWidgetManager::storageNodeName()
{
	return QLatin1String("ChatWindows");
}

QString ChatWidgetManager::storageItemNodeName()
{
	return QLatin1String("Chat");
}

void ChatWidgetManager::closeAllWindows()
{
	ensureStored();

	QHash<Chat, ChatWidget *>::iterator i = Chats.begin();
	while (i != Chats.end())
	{
		ChatWindow *window = qobject_cast<ChatWindow *>(i.value()->window());
		if (window)
		{
			i = Chats.erase(i);
			delete window;
			continue;
		}
		++i;
	}
}

void ChatWidgetManager::load()
{
	if (!isValidStorage())
		return;

	StorableStringList::load();

	foreach (const QString &uuid, content())
	{
		QUuid chatId(uuid);

		if (chatId.isNull())
			continue;

		byChat(ChatManager::instance()->byUuid(chatId), true);
	}
}

void ChatWidgetManager::store()
{
	if (!isValidStorage())
		return;

	StringList.clear();

	if (config_file.readBoolEntry("Chat", "SaveOpenedWindows", true))
	{
		for (QHash<Chat , ChatWidget *>::const_iterator it = Chats.constBegin(), end = Chats.constEnd(); it != end; ++it)
		{
			Protocol *protocolHandler = it.key().chatAccount().protocolHandler();
			if (!protocolHandler || !protocolHandler->protocolFactory() || !qobject_cast<ChatWindow *>(it.value()->window()))
					continue;

			StringList.append(it.key().uuid().toString());
		}
	}

	StorableStringList::store();
}

const QHash<Chat, ChatWidget *> & ChatWidgetManager::chats() const
{
	return Chats;
}

ChatWidget * ChatWidgetManager::byChat(const Chat &chat, const bool create)
{
	if (!chat)
		return 0;

	if (Chats.contains(chat))
		return Chats.value(chat);

	if (!create)
		return 0;

	ChatWidget * const chatWidget = createChatWidget(chat);
	if (chatWidget)
		Chats.insert(chat, chatWidget);

	return chatWidget;
}

ChatWidget * ChatWidgetManager::createChatWidget(const Chat &chat)
{
	if (!chat)
		return 0;

	ChatWidget *chatWidget = new ChatWidget(chat);
	Chats.insert(chat, chatWidget);

	connect(chatWidget, SIGNAL(widgetDestroyed()), this, SLOT(chatWidgetDestroyed()));

	bool handled = false;
	emit handleNewChatWidget(chatWidget, handled);
	if (!handled)
	{
		ChatWindow *chatWindow = new ChatWindow(chatWidget);
		chatWidget->setContainer(chatWindow);
		chatWindow->show();
	}

//	if (chatWidget->chat().contacts().count() == 1)
//	{
//		Contact contact = chatWidget->chat().contacts().toContact();
//		BuddyPreferredManager::instance()->updatePreferred(contact.ownerBuddy());
//	}

	emit chatWidgetCreated(chatWidget);

	const QList<Message> &messages = readPendingMessages(chat);
	if (!messages.isEmpty())
		// TODO: Lame API
		if (0 == chatWidget->countMessages())
			chatWidget->appendMessages(messages);

	return chatWidget;
}

void ChatWidgetManager::chatWidgetDestroyed()
{
	ChatWidget *chatWidget = qobject_cast<ChatWidget *>(sender());
	if (!chatWidget)
		return;

	disconnect(chatWidget, SIGNAL(widgetDestroyed()), this, SLOT(chatWidgetDestroyed()));

	if (!Chats.contains(chatWidget->chat()))
		return;

	Chats.remove(chatWidget->chat());

//	if (chatwidget->chat().contacts().count() == 1)
//	{
//		Contact contact = chatwidget->chat().contacts().toContact();
//		BuddyPreferredManager::instance()->updatePreferred(contact.ownerBuddy());
//	}

	emit chatWidgetDestroying(chatWidget);
}

QList<Message> ChatWidgetManager::readPendingMessages(const Chat &chat)
{
	const Chat &aggregateChat = AggregateChatManager::instance()->aggregateChat(chat);
	const QVector<Message> &pendingMessages = PendingMessagesManager::instance()->pendingMessagesForChat(
	            aggregateChat ? aggregateChat : chat);

	QList<Message> messages;
	foreach (Message message, pendingMessages)
	{
		messages.append(message);
		message.setPending(false);
		PendingMessagesManager::instance()->removeItem(message);
	}

	return messages;
}

void ChatWidgetManager::closeChat(const Chat &chat)
{
	ChatWidget * const chatWidget = byChat(chat, false);
	if (chatWidget && chatWidget->container())
		chatWidget->container()->closeChatWidget(chatWidget);
}

void ChatWidgetManager::closeAllChats(const Buddy &buddy)
{
	foreach (const Contact &contact, buddy.contacts())
	{
		const Chat &chat = ChatManager::instance()->findChat(ContactSet(contact), false);
		if (chat)
			closeChat(chat);
	}
}

void ChatWidgetManager::configurationUpdated()
{
	OpenChatOnMessage = config_file.readBoolEntry("Chat", "OpenChatOnMessage");
	AutoRaise = config_file.readBoolEntry("General","AutoRaise");
	OpenChatOnMessageWhenOnline = config_file.readBoolEntry("Chat", "OpenChatOnMessageWhenOnline");;
}

// TODO 0.10.0:, move to core or somewhere else
void ChatWidgetManager::messageReceived(const Message &message)
{
	const Chat &chat = message.messageChat();
	ChatWidget *chatWidget = byChat(chat, false);
	if (chatWidget)
		chatWidget->appendMessage(message);
	else
	{
		if (AutoRaise)
			_activateWindow(Core::instance()->kaduWindow());

		if (OpenChatOnMessage)
		{
			Protocol *handler = message.messageChat().chatAccount().protocolHandler();
			if (OpenChatOnMessageWhenOnline && (!handler || (handler->status().group() != StatusTypeGroupOnline)))
			{
				qApp->alert(Core::instance()->kaduWindow());
				message.setPending(true);
				PendingMessagesManager::instance()->addItem(message);
				return;
			}

			// TODO: it is lame
			chatWidget = byChat(chat, true);

			if (chatWidget)
				chatWidget->appendMessage(message);
		}
		else
		{
			qApp->alert(Core::instance()->kaduWindow());
			message.setPending(true);
			PendingMessagesManager::instance()->addItem(message);
		}
	}
}

void ChatWidgetManager::messageSent(const Message &message)
{
	const Chat &chat = message.messageChat();
	ChatWidget * const chatWidget = byChat(chat, true);
	if (!chatWidget)
		return;

	chatWidget->appendMessage(message);
}
