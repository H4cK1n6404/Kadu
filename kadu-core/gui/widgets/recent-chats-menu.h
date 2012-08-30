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

#ifndef RECENT_CHATS_MENU_H
#define RECENT_CHATS_MENU_H

#include <QtGui/QMenu>

#include "exports.h"

class KADUAPI RecentChatsMenu : public QMenu
{
	Q_OBJECT

	bool RecentChatsMenuNeedsUpdate;

public:
	RecentChatsMenu(QWidget *parent);
	virtual ~RecentChatsMenu();

private slots:
	void invalidate();
	void checkIfListAvailable();
	void update();
	void iconThemeChanged();

signals:
	void chatsListAvailable(bool available);
};

#endif // RECENT_CHATS_MENU_H