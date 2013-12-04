/*
 * %kadu copyright begin%
 * Copyright 2013 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include "chat-widget-repository.h"

#include "gui/widgets/chat-widget/chat-widget.h"

ChatWidgetRepository::ChatWidgetRepository(QObject *parent) :
		QObject(parent)
{
}

ChatWidgetRepository::~ChatWidgetRepository()
{
	// neeed to emit signals on finish
	while (!m_widgets.empty())
		removeChatWidget((*m_widgets.begin()).second.get());
}

ChatWidgetRepositoryIterator ChatWidgetRepository::begin()
{
	return ChatWidgetRepositoryIterator{m_widgets.begin()};
}

ChatWidgetRepositoryIterator ChatWidgetRepository::end()
{
	return ChatWidgetRepositoryIterator{m_widgets.end()};
}

void ChatWidgetRepository::addChatWidget(std::unique_ptr<ChatWidget> chatWidget)
{
	if (!chatWidget || hasWidgetForChat(chatWidget.get()->chat()))
		return;

	emit chatWidgetAdded(chatWidget.get());
	m_widgets.insert(std::make_pair(chatWidget->chat(), std::move(chatWidget)));
}

void ChatWidgetRepository::removeChatWidget(ChatWidget *chatWidget)
{
	if (!chatWidget || !hasWidgetForChat(chatWidget->chat()))
		return;

	emit chatWidgetRemoved(chatWidget);
	m_widgets.erase(chatWidget->chat());
}

bool ChatWidgetRepository::hasWidgetForChat(const Chat &chat) const
{
	return m_widgets.end() != m_widgets.find(chat);
}

ChatWidget * ChatWidgetRepository::widgetForChat(const Chat &chat)
{
	if (!chat || !hasWidgetForChat(chat))
		return nullptr;

	return m_widgets.at(chat).get();
}

#include "moc_chat-widget-repository.cpp"