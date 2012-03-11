/*
 * %kadu copyright begin%
 * Copyright 2010, 2011 Piotr Galiszewski (piotr.galiszewski@kadu.im)
 * Copyright 2009, 2010, 2011 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * Copyright 2010 Bartosz Brachaczek (b.brachaczek@gmail.com)
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

#include "buddies/model/buddy-list-model.h"
#include "buddies/buddy-manager.h"
#include "core/core.h"

#include "buddy-manager-adapter.h"

BuddyManagerAdapter::BuddyManagerAdapter(BuddyListModel *model) :
		QObject(model), Model(model), IncludeMyself(false)
{
	Model->setBuddyList(BuddyManager::instance()->items().toList());

	BuddyManager *manager = BuddyManager::instance();
	connect(manager, SIGNAL(buddyAdded(Buddy)),
			this, SLOT(buddyAdded(Buddy)), Qt::DirectConnection);
	connect(manager, SIGNAL(buddyRemoved(Buddy)),
			this, SLOT(buddyRemoved(Buddy)), Qt::DirectConnection);
}

BuddyManagerAdapter::~BuddyManagerAdapter()
{

	BuddyManager *manager = BuddyManager::instance();
	disconnect(manager, SIGNAL(buddyAdded(Buddy)),
			this, SLOT(buddyAdded(Buddy)));
	disconnect(manager, SIGNAL(buddyAboutToBeRemoved(Buddy)),
			this, SLOT(buddyRemoved(Buddy)));
}

void BuddyManagerAdapter::buddyAdded(const Buddy &buddy)
{
	Model->addBuddy(buddy);
}

void BuddyManagerAdapter::buddyRemoved(const Buddy &buddy)
{
	Model->removeBuddy(buddy);
}

void BuddyManagerAdapter::setIncludeMyself(bool includeMyself)
{
	if (IncludeMyself == includeMyself)
		return;

	IncludeMyself = includeMyself;
	if (IncludeMyself)
		Model->addBuddy(Core::instance()->myself());
	else
		Model->removeBuddy(Core::instance()->myself());
}