/*
 * %kadu copyright begin%
 * Copyright 2011 Piotr Galiszewski (piotr.galiszewski@kadu.im)
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

#include "formatted-string/composite-formatted-string.h"
#include "formatted-string/formatted-string-factory.h"
#include "message/message-filter-service.h"
#include "protocols/protocol.h"
#include "protocols/services/chat-service.h"
#include "services/message-transformer-service.h"

#include "message-manager.h"

MessageManager::MessageManager(QObject *parent) :
		QObject{parent}
{
	// TODO: move somewhere else?
	qRegisterMetaType<Message>();

	triggerAllAccountsRegistered();
}

MessageManager::~MessageManager()
{
}

void MessageManager::setMessageFilterService(MessageFilterService *messageFilterService)
{
	m_messageFilterService = messageFilterService;
}

void MessageManager::setMessageTransformerService(MessageTransformerService *messageTransformerService)
{
	m_messageTransformerService = messageTransformerService;
}

void MessageManager::setFormattedStringFactory(FormattedStringFactory *formattedStringFactory)
{
	m_formattedStringFactory = formattedStringFactory;
}

void MessageManager::done()
{
	triggerAllAccountsUnregistered();
}

void MessageManager::accountRegistered(Account account)
{
	Protocol *protocol = account.protocolHandler();
	if (!protocol)
		return;

	ChatService *chatService = protocol->chatService();
	if (!chatService)
		return;

	connect(chatService, SIGNAL(messageReceived(const Message &)),
	        this, SLOT(messageReceivedSlot(const Message &)));
	connect(chatService, SIGNAL(messageSent(const Message &)),
	        this, SIGNAL(messageSent(const Message &)));
}

void MessageManager::accountUnregistered(Account account)
{
	Protocol *protocol = account.protocolHandler();
	if (!protocol)
		return;

	ChatService *chatService = protocol->chatService();
	if (!chatService)
		return;

	disconnect(chatService, 0, this, 0);
}

void MessageManager::messageReceivedSlot(const Message &message)
{
	Message transformedMessage = m_messageTransformerService
			? m_messageTransformerService.data()->transform(message)
			: message;

	if (m_messageFilterService)
		if (!m_messageFilterService.data()->acceptMessage(transformedMessage))
			return;

	emit messageReceived(transformedMessage);
}

bool MessageManager::sendMessage(const Chat &chat, const QString &content, bool silent)
{
	if (!m_formattedStringFactory)
		return false;

	return sendMessage(chat, m_formattedStringFactory.data()->fromText(content), silent);
}

Message MessageManager::createOutgoingMessage(const Chat &chat, std::unique_ptr<FormattedString> &&content)
{
	Message message = Message::create();
	message.setMessageChat(chat);
	message.setType(MessageTypeSent);
	message.setMessageSender(chat.chatAccount().accountContact());
	message.setContent(std::move(content));
	message.setSendDate(QDateTime::currentDateTime());
	message.setReceiveDate(QDateTime::currentDateTime());

	return message;
}

bool MessageManager::sendMessage(const Chat &chat, std::unique_ptr<FormattedString> &&content, bool silent)
{
	Protocol *protocol = chat.chatAccount().protocolHandler();
	if (!protocol || !protocol->chatService())
		return false;

	Message message = createOutgoingMessage(chat, std::move(content));
	if (m_messageFilterService && !m_messageFilterService.data()->acceptMessage(message))
		return false;

	Message transformedMessage = m_messageTransformerService
			? m_messageTransformerService.data()->transform(message)
			: message;

	bool sent = protocol->chatService()->sendMessage(transformedMessage);
	if (sent && !silent)
		emit messageSent(transformedMessage);

	return sent;
}

bool MessageManager::sendRawMessage(const Chat &chat, const QByteArray &content)
{
	Protocol *protocol = chat.chatAccount().protocolHandler();
	if (!protocol || !protocol->chatService())
		return false;

	return protocol->chatService()->sendRawMessage(chat, content);
}

#include "moc_message-manager.cpp"
