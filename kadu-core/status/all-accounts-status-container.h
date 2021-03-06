/*
 * %kadu copyright begin%
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

#pragma once

#include "accounts/accounts-aware-object.h"
#include "status/status-container.h"

#include <QtCore/QPointer>
#include <injeqt/injeqt.h>

class AccountManager;
class Account;
class StatusConfigurationHolder;

/**
 * @addtogroup Status
 * @{
 */

/**
 * @class AllAccountsStatusContainer
 * @short Grouping status container for all accounts.
 * @see StatusContainerManager
 *
 * This class is status container wrapper for all registered accounts. All its getters are delegated to best account
 * status container (@see AccountManager::bestAccount). All its setters are delegated to all accounts.
 */
class KADUAPI AllAccountsStatusContainer : public StatusContainer, public AccountsAwareObject
{
    Q_OBJECT

protected:
    /**
     * @short Method called when new account is registered.
     *
     * This method is called every time new account is registered. New account is added to list of
     * status containers handled by this object. Signal statusUpdated is emited, as this new account
     * could change AccountManager::bestAccount and result of all getters results.
     */
    virtual void accountAdded(Account account) override;

    /**
     * @short Method called when account is unregistered.
     *
     * This method is called every time new account is unregistered. This account is removed from list of
     * status containers handled by this object. Signal statusUpdated is emited, as removal of this account
     * could change AccountManager::bestAccount and result of all getters results.
     */
    virtual void accountRemoved(Account account) override;

public:
    Q_INVOKABLE explicit AllAccountsStatusContainer(QObject *parent = nullptr);
    virtual ~AllAccountsStatusContainer();

    /**
     * @short Returns empty string as this container name.
     * @return empty string as this container name
     *
     * Returns empty string as this container name.
     */
    virtual QString statusContainerName() override
    {
        return QString();
    }

    /**
     * @short Sets status on all registered accounts.
     * @param status status to set
     * @param source source of change
     *
     * Sets status on all registered accounts.
     */
    virtual void setStatus(Status status, StatusChangeSource source) override;

    /**
     * @short Return best account status.
     * @return best account status
     *
     * Return best account status.
     * For definition of best account see @see AccountManager::bestAccount.
     */
    virtual Status status() override;

    /**
     * @short Return true if best account status setting is in progress.
     * @return true if best account status setting is in progress
     *
     * Return true if best account status setting is in progress.
     * For definition of best account see @see AccountManager::bestAccount.
     */
    virtual bool isStatusSettingInProgress() override;

    /**
     * @short Return maximum length of description on best account.
     * @return maximum length of description on best account
     *
     * Return maximum length of description on best account.
     * For definition of best account see @see AccountManager::bestAccount.
     */
    virtual int maxDescriptionLength() override;

    /**
     * @short Return status icon of best account.
     * @return status icon of best accountt
     *
     * Return maximum length of description on best account.
     * Return status icon of best account.
     */
    virtual KaduIcon statusIcon() override;

    /**
     * @short Return status icon of given status using best account icons.
     * @param status to get icon for
     * @return status icon of given status using best account icons
     *
     * Return status icon of given status using best account icons.
     * Return status icon of best account.
     */
    virtual KaduIcon statusIcon(const Status &status) override;

    /**
     * @short Return list of supported status types for best account.
     * @return list of supported status types for best account
     *
     * Return list of supported status types for best account.
     * Return status icon of best account.
     */
    virtual QList<StatusType> supportedStatusTypes() override;

    /**
     * @short Return best account stored status.
     * @return best account stored status
     *
     * Return best account stored status.
     * Return status icon of best account.
     */
    virtual Status loadStatus() override;

    /**
     * @short Stores given status in all accounts.
     * @param status status to store
     *
     * Stores given status in all accounts.
     */
    virtual void storeStatus(Status status) override;

private:
    QPointer<AccountManager> m_accountManager;
    QPointer<StatusConfigurationHolder> m_statusConfigurationHolder;

    QList<Account> Accounts;
    Status LastSetStatus;

private slots:
    INJEQT_SET void setAccountManager(AccountManager *accountManager);
    INJEQT_SET void setStatusConfigurationHolder(StatusConfigurationHolder *statusConfigurationHolder);
    INJEQT_INIT void init();
    INJEQT_DONE void done();

    void protocolHandlerChanged(Account account);
};

/**
 * @}
 */
