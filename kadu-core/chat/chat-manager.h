/*
 * %kadu copyright begin%
 * Copyright 2010, 2011 Piotr Galiszewski (piotr.galiszewski@kadu.im)
 * Copyright 2009, 2012 Wojciech Treter (juzefwt@gmail.com)
 * Copyright 2009 Michał Podsiadlik (michal@kadu.net)
 * Copyright 2009 Bartłomiej Zimoń (uzi18@o2.pl)
 * Copyright 2010 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2009, 2010, 2011, 2012, 2013, 2014 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include <QtCore/QObject>
#include <QtCore/QUuid>
#include <QtCore/QVector>
#include <injeqt/injeqt.h>

#include "message/message.h"
#include "storage/manager.h"

#include "chat.h"

#include "exports.h"

/**
 * @defgroup Chat
 *
 * Chats are basic concept in Kadu. Chat can define any conversation beetwen people. In core Kadu code
 * exists two kinds of chats:
 * <ul>
 *  <li>contact - chat with one person</li>
 *  <li>confernece - chat with more that one person</li>
 * </ul>
 *
 * Other kinds of chats can be defined (e.g. IRC chat would be defined by server and IRC room name,
 * not by the people that are in the conversation for a given moment).
 *
 * All currently defined kinds of chats are stored in @link ChatTypeManger @endlink class. Plugins
 * can add new kind of chats and register them in manager. Every object that needs to know about
 * all installed chat types should derive from @link ChatTypeAwareObject @endlink.
 *
 * @link ChatManager @endlink stores all chats that were used in program. Every chat type has associated
 * @link ChatDetails @endlink class that stored data specific to this chat type. ChatManager can load
 * only data common for all chats, remaining data can only by loaded by details class. Two details classes
 * are defined in core: @link ChatDetailsContact @endlink and @link ChatDetailsContactSet @endlink
 */

/**
 * @addtogroup Chat
 * @{
 */

class Account;
class BuddySet;
class ChatStorage;
class ConfigurationApi;
class ConfigurationManager;
class UnreadMessageRepository;

/**
 * @class ChatManager
 * @short Manager for all chats used in application.
 *
 * This class manages all chats used in application. It derives from @link Manager @endlink class.
 * All new chats of standard types (contact and contact set) should be created by @link findChat @endlink
 * method. No two chats will have the same contacts and no conflicts will be made.
 *
 * Signals @link chatAboutToBeAdded @endlink, @link chatAdded @endlink, @link chatAboutToBeRemoved @endlink
 * and @link chatRemoved @endlink are emitted when details for given chat are loaded. It means
 * that whole data is available for other objects to use.
 *
 * Use @link items @endlink method to get access to all chats will full data, use @link allItems @endlink
 * to get access to all chats, even with only basic data.
 */
class KADUAPI ChatManager : public Manager<Chat>
{
    Q_OBJECT

public:
    explicit ChatManager(QObject *parent = nullptr);
    virtual ~ChatManager();

    /**
     * @short Return list of chats assigned to given account.
     * @param account account of returned chats
     *
     * If account is null then this method returns empty vector.
     */
    virtual QVector<Chat> chats(const Account &account) = 0;

    virtual Chat byDisplay(const QString &display) = 0;

protected:
    virtual void itemAboutToBeAdded(Chat item) override;
    virtual void itemAdded(Chat item) override;
    virtual void itemAboutToBeRemoved(Chat item) override;
    virtual void itemRemoved(Chat item) override;

signals:
    /**
     * @short Emitted just before chat with full data is added to manager.
     * @param chat added chat
     *
     * Signal is emitted just before chat with full data is added to manager.
     * It is also emitted after full data is loaded for given chat but before it
     * is added.
     */
    void chatAboutToBeAdded(Chat chat);

    /**
     * @short Emitted just after chat with full data is added to manager.
     * @param chat added chat
     *
     * Signal is emitted just after chat with full data is added to manager.
     * It is also emitted after full data is loaded for given chat and after it
     * is added.
     */
    void chatAdded(Chat chat);

    /**
     * @short Emitted just before chat with full data is removed from manager.
     * @param chat removed chat
     *
     * Signal is emitted just before chat removed from manager. Removed chat
     * does not have to have full data. This signal is also removed after
     * chat loses its full data.
     */
    void chatAboutToBeRemoved(Chat chat);

    /**
     * @short Emitted just after chat with full data is removed from manager.
     * @param chat removed chat
     *
     * Signal is emitted just after chat removed from manager. Removed chat
     * does not have to have full data. This signal is also removed after
     * chat loses its full data.
     */
    void chatRemoved(Chat chat);

    /**
     * @short Emitted when data of given chat has changed.
     * @param chat updated chat
     */
    void chatUpdated(const Chat &chat);

    /**
     * @short Signal emited when given chat has been opened.
     * @param chat opened chat
     */
    void chatOpened(const Chat &chat);

    /**
     * @short Signal emited when given chat has been closed.
     * @param chat closed chat
     */
    void chatClosed(const Chat &chat);
};

/**
 * @}
 */
