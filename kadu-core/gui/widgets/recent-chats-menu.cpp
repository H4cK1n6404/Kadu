/*
 * %kadu copyright begin%
 * Copyright 2012 Wojciech Treter (juzefwt@gmail.com)
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

#include "chat/model/chat-data-extractor.h"
#include "chat/recent-chat-manager.h"
#include "chat/type/chat-type-manager.h"
#include "gui/widgets/chat-widget-manager.h"
#include "gui/widgets/chat-widget.h"
#include "icons/icons-manager.h"
#include "icons/kadu-icon.h"

#include "recent-chats-menu.h"


RecentChatsMenu::RecentChatsMenu(QWidget *parent = 0) :
		QMenu(parent)
{
	setIcon(KaduIcon("internet-group-chat").icon());
	setTitle(tr("Recent chats"));

	RecentChatsMenuNeedsUpdate = true;

	connect(IconsManager::instance(), SIGNAL(themeChanged()), this, SLOT(iconThemeChanged()));
	connect(ChatWidgetManager::instance(), SIGNAL(chatWidgetCreated(ChatWidget*)), this, SLOT(invalidate()));
	connect(ChatWidgetManager::instance(), SIGNAL(chatWidgetDestroying(ChatWidget*)), this, SLOT(invalidate()));
	connect(RecentChatManager::instance(), SIGNAL(recentChatAdded(Chat)), this, SLOT(invalidate()));
	connect(RecentChatManager::instance(), SIGNAL(recentChatRemoved(Chat)), this, SLOT(invalidate()));
	connect(this, SIGNAL(aboutToShow()), this, SLOT(update()));
}

RecentChatsMenu::~RecentChatsMenu()
{
}

void RecentChatsMenu::invalidate()
{
	RecentChatsMenuNeedsUpdate = true;

	checkIfListAvailable();
}

void RecentChatsMenu::checkIfListAvailable()
{
	//check if all recent chats are opened -> disable button
	foreach (const Chat &chat, RecentChatManager::instance()->recentChats())
		if (!ChatWidgetManager::instance()->byChat(chat, false))
		{
			emit chatsListAvailable(true);
			return;
		}

	emit chatsListAvailable(false);
}

void RecentChatsMenu::update()
{
	if (!RecentChatsMenuNeedsUpdate)
		return;

	clear();

	foreach (const Chat &chat, RecentChatManager::instance()->recentChats())
		if (!ChatWidgetManager::instance()->byChat(chat, false))
		{
			ChatType *type = ChatTypeManager::instance()->chatType(chat.type());
			QAction *action = new QAction(type ? type->icon().icon() : QIcon(),
			                              ChatDataExtractor::data(chat, Qt::DisplayRole).toString(),
			                              this);
			action->setData(QVariant::fromValue<Chat>(chat));
			this->addAction(action);
		}

	emit chatsListAvailable(!actions().isEmpty());

	RecentChatsMenuNeedsUpdate = false;
}

void RecentChatsMenu::iconThemeChanged()
{
	setIcon(KaduIcon("internet-group-chat").icon());
}
