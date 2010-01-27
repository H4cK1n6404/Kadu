/*
 * %kadu copyright begin%
 * Copyright 2009, 2010 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * Copyright 2009 Piotr Galiszewski (piotrgaliszewski@gmail.com)
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

#include "configuration/configuration-manager.h"
#include "contacts/contact.h"
#include "contacts/contact-shared.h"
#include "core/core.h"
#include "protocols/protocol.h"
#include "protocols/protocol-factory.h"
#include "debug.h"

#include "contact-manager.h"

ContactManager * ContactManager::Instance = 0;

ContactManager * ContactManager::instance()
{
	if (0 == Instance)
		Instance = new ContactManager();

	return Instance;
}

ContactManager::ContactManager()
{
}

ContactManager::~ContactManager()
{
}

void ContactManager::idChanged(const QString &oldId)
{
	Contact contact(sender());
	if (!contact.isNull())
		emit contactIdChanged(contact, oldId);
}

void ContactManager::aboutToBeDetached()
{
	Contact contact(sender());
	if (!contact.isNull())
		emit contactAboutToBeDetached(contact);
}

void ContactManager::detached()
{
	Contact contact(sender());
	if (!contact.isNull())
		emit contactDetached(contact);
}

void ContactManager::aboutToBeAttached()
{
	Contact contact(sender());
	if (!contact.isNull())
		emit contactAboutToBeAttached(contact);
}

void ContactManager::attached()
{
	Contact contact(sender());
	if (!contact.isNull())
		emit contactAttached(contact);
}

void ContactManager::itemAboutToBeRegistered(Contact item)
{
	emit contactAboutToBeAdded(item);
}

void ContactManager::itemRegistered(Contact item)
{
	emit contactAdded(item);
	connect(item, SIGNAL(idChanged(const QString &)), this, SLOT(idChanged(const QString &)));
	connect(item, SIGNAL(aboutToBeDetached()), this, SLOT(aboutToBeDetached()));
	connect(item, SIGNAL(detached()), this, SLOT(detached()));
	connect(item, SIGNAL(aboutToBeAttached()), this, SLOT(aboutToBeAttached()));
	connect(item, SIGNAL(attached()), this, SLOT(attached()));
}

void ContactManager::itemAboutToBeUnregisterd(Contact item)
{
	emit contactAboutToBeRemoved(item);
	disconnect(item, SIGNAL(idChanged(const QString &)), this, SLOT(idChanged(const QString &)));
}

void ContactManager::itemUnregistered(Contact item)
{
	emit contactRemoved(item);
}

void ContactManager::detailsLoaded(Contact item)
{
	if (!item.isNull())
		registerItem(item);
}

void ContactManager::detailsUnloaded(Contact item)
{
	if (!item.isNull())
		unregisterItem(item);
}

Contact ContactManager::byId(Account account, const QString &id, NotFoundAction action)
{
	ensureLoaded();

	if (id.isEmpty() || account.isNull())
		return Contact::null;

	foreach (const Contact &contact, allItems())
		if (account == contact.contactAccount() && id == contact.id())
			return contact;

	if (action == ActionReturnNull)
		return Contact::null;

	Contact contact = Contact::create();
	contact.setContactAccount(account);
	contact.setId(id);

	if (action == ActionCreateAndAdd)
		addItem(contact);

	Protocol *protocolHandler = account.protocolHandler();
	if (!protocolHandler)
		return contact;

	ProtocolFactory *factory = protocolHandler->protocolFactory();
	if (!factory)
		return contact;

	ContactDetails *details = factory->createContactDetails(contact);
	details->setState(StateNotLoaded);
	contact.setDetails(details);

	return contact;
}

QList<Contact> ContactManager::contacts(Account account)
{
	ensureLoaded();

	QList<Contact> contacts;

	if (account.isNull())
		return contacts;

	foreach (const Contact &contact, allItems())
		if (account == contact.contactAccount())
			contacts.append(contact);

	return contacts;
}
