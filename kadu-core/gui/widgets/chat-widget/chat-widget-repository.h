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

#pragma once

#include "chat/chat.h"
#include "gui/widgets/chat-widget/chat-widget-repository-iterator.h"
#include "exports.h"

#include <map>
#include <memory>
#include <QtCore/QObject>

class ChatWidget;

/**
 * @addtogroup Gui
 * @{
 */

/**
 * @class ChatWidgetRepository
 * @short Repository of ChatWidget instances.
 * @note This class is experimental in Kadu, watch for unique_ptr!
 *
 * This repository holds instances of ChatWidget. All instances are owned
 * by this class and can only be deleted by it.
 *
 * Instances are added by addChatWidget(std::unique_ptr&lt;ChatWidget&gt;) which
 * transfers ownership of chat widget to repository. It also emits
 * chatWidgetAdded(ChatWidget*) signals.
 *
 * Instances are removed by removeChatWidget(ChatWidget*) methods. Signal
 * chatWidgetRemoved(ChatWidget*) is removed and chat widget is destroyed.
 *
 * Instances can be enumerated as ChatWidget* in standard for-range loop as
 * begin() and end() iterators are provided.
 */
class KADUAPI ChatWidgetRepository : public QObject
{
	Q_OBJECT

public:
	explicit ChatWidgetRepository(QObject *parent = nullptr);
	virtual ~ChatWidgetRepository();

	/**
	 * @short Begin iterator that returns ChatWidget *.
	 */
	ChatWidgetRepositoryIterator begin();

	/**
	 * @short Begin iterator that returns ChatWidget *.
	 */
	ChatWidgetRepositoryIterator end();

	/**
	 * @short Add new chatWidget to repository.
	 *
	 * Add new chatWidget to repository only if it is valid and chat widget for given
	 * chat is not already in repository. In other case argument is destroyed.
	 *
	 * In case of successfull addition ownership of chatWidget is taken by repository
	 * and chatWidgetAdded(ChatWidget*) signal is emitted.
	 */
	void addChatWidget(std::unique_ptr<ChatWidget> chatWidget);

	/**
	 * @short Remove chatWidget from repository and destroy it.
	 *
	 * Remove chatWidget from repository only if it is  already in repository.
	 * Signal chatWidgetRemoved(ChatWidget*) is emitted after successfull removal.
	 * Then provided chat widget is destroyed.
	 */
	void removeChatWidget(ChatWidget *chatWidget);

	/**
	 * @short Return true if repository has chat widget for given chat.
	 */
	bool hasWidgetForChat(const Chat &chat) const;

	/**
	 * @short Return ChatWidget for given chat.
	 * @param chat chat to get ChatWidget for
	 * @return ChatWidget for given chat
	 *
	 * If chat is null then nullptr is returned. If repository does contain ChatWidget then
	 * it is returned. Else nullptr is returned.
	 */
	ChatWidget * widgetForChat(const Chat &chat);

signals:
	/**
	 * @short Signal emitted when new ChatWidget was created for this repository.
	 * @param chatWidget newly created ChatWidget instance
	 */
	void chatWidgetAdded(ChatWidget *chatWidget);

	/**
	 * @short Signal emitted when new ChatWidget was destroyed in this repository.
	 * @param chatWidget just destroyed ChatWidget instance
	 */
	void chatWidgetRemoved(ChatWidget *chatWidget);

private:
	std::map<Chat, std::unique_ptr<ChatWidget>> m_widgets;

};

inline ChatWidgetRepositoryIterator begin(ChatWidgetRepository *chatWidgetRepository)
{
	return chatWidgetRepository->begin();
}

inline ChatWidgetRepositoryIterator end(ChatWidgetRepository *chatWidgetRepository)
{
	return chatWidgetRepository->end();
}

/**
 * @}
 */