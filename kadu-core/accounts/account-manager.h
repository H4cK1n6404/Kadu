/*
 * %kadu copyright begin%
 * Copyright 2009, 2011 Piotr Galiszewski (piotr.galiszewski@kadu.im)
 * Copyright 2011 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2009, 2010, 2011, 2014 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include "accounts/account.h"
#include "exports.h"
#include "protocols/protocol-factory.h"
#include "protocols/protocol.h"
#include "storage/manager.h"

#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QUuid>
#include <injeqt/injeqt.h>

class AccountStorage;
class BuddyManager;
class ChatManager;
class ConfigurationApi;
class ConfigurationManager;
class ContactManager;
class InjectedFactory;
class Myself;
class Status;

class KADUAPI AccountManager : public Manager<Account>
{
    Q_OBJECT

    QPointer<AccountStorage> m_accountStorage;
    QPointer<BuddyManager> m_buddyManager;
    QPointer<ChatManager> m_chatManager;
    QPointer<ConfigurationManager> m_configurationManager;
    QPointer<ContactManager> m_contactManager;
    QPointer<InjectedFactory> m_injectedFactory;
    QPointer<Myself> m_myself;

private slots:
    INJEQT_SET void setAccountStorage(AccountStorage *accountStorage);
    INJEQT_SET void setBuddyManager(BuddyManager *buddyManager);
    INJEQT_SET void setChatManager(ChatManager *chatManager);
    INJEQT_SET void setConfigurationManager(ConfigurationManager *configurationManager);
    INJEQT_SET void setContactManager(ContactManager *contactManager);
    INJEQT_SET void setInjectedFactory(InjectedFactory *injectedFactory);
    INJEQT_SET void setMyself(Myself *myself);
    INJEQT_INIT void init();
    INJEQT_DONE void done();

    void passwordProvided(const QVariant &data, const QString &password, bool permament);
    void protocolHandlerChanged(Account);

    void accountDataUpdated();

protected:
    virtual Account loadStubFromStorage(const std::shared_ptr<StoragePoint> &storagePoint) override;

    virtual void itemAboutToBeAdded(Account item) override;
    virtual void itemAdded(Account item) override;
    virtual void itemAboutToBeRemoved(Account item) override;
    virtual void itemRemoved(Account item) override;

    virtual void loaded() override;

public:
    Q_INVOKABLE explicit AccountManager(QObject *parent = nullptr);
    virtual ~AccountManager();

    template <template <class> class Container>
    static Account bestAccount(const Container<Account> &accounts)
    {
        Account result;
        if (accounts.isEmpty())
            return result;

        for (auto const &account : accounts)
            if (account.data())
            {
                // TODO: hack
                auto p = protocol(account);
                bool newConnected = p && p->isConnected();
                bool oldConnected = false;
                if (result)
                    oldConnected = result.data()->protocolHandler() && result.data()->protocolHandler()->isConnected();

                if (!result || (newConnected && !oldConnected) || (account.protocolName() == QStringLiteral("gadu") &&
                                                                   result.protocolName() != QStringLiteral("gadu")))
                {
                    result = account;
                    if (newConnected && result.protocolName() == QStringLiteral("gadu"))
                        break;
                }
            }

        return result;
    }

    virtual QString storageNodeName() override
    {
        return QStringLiteral("Accounts");
    }
    virtual QString storageNodeItemName() override
    {
        return QStringLiteral("Account");
    }

    Account defaultAccount();
    Account bestAccount();

    const QVector<Account> byIdentity(Identity identity);
    Account byId(const QString &protocolName, const QString &id);
    const QVector<Account> byProtocolName(const QString &name);

    void removeAccountAndBuddies(Account account);

public slots:
    void providePassword(Account account);

signals:
    void accountAboutToBeAdded(Account);
    void accountAdded(Account);
    void accountAboutToBeRemoved(Account);
    void accountRemoved(Account);

    void accountLoadedStateChanged(Account);

    void accountUpdated(Account);
};
