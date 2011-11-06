/*
 * %kadu copyright begin%
 * Copyright 2011 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include "gui/actions/base-action-data-source.h"
#include "gui/widgets/chats-tree-view-delegate.h"
#include "model/roles.h"
#include "model/model-index-list-converter.h"

#include "chats-tree-view.h"

ChatsTreeView::ChatsTreeView(QWidget *parent) :
		KaduTreeView(parent), ActionData(new BaseActionDataSource())
{
	setItemDelegate(new ChatsTreeViewDelegate(this));
}

ChatsTreeView::~ChatsTreeView()
{
	delete ActionData;
	ActionData = 0;
}

void ChatsTreeView::setModel(QAbstractItemModel *model)
{
	KaduTreeView::setModel(model);

	connect(selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
	        this, SLOT(updateActionData()));
	updateActionData();
}

void ChatsTreeView::updateActionData()
{
	ModelIndexListConverter converter(selectedIndexes());

	ActionData->blockChangedSignal();

	ActionData->setRoles(converter.roles());
	ActionData->setBuddies(converter.buddies());
	ActionData->setContacts(converter.contacts());
	ActionData->setChat(converter.chat());

	ActionData->unblockChangedSignal();
}

ActionDataSource * ChatsTreeView::actionDataSource()
{
	return ActionData;
}
