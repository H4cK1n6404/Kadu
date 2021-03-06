/*
 * %kadu copyright begin%
 * Copyright 2013 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#ifndef ACCOUNT_CONFIGURATION_WIDGET_TAB_ADAPTER_H
#define ACCOUNT_CONFIGURATION_WIDGET_TAB_ADAPTER_H

#include "exports.h"
#include <QtCore/QObject>

class QTab;
class QTabWidget;

class AccountConfigurationWidget;
class AccountEditWidget;

class KADUAPI AccountConfigurationWidgetTabAdapter : public QObject
{
    Q_OBJECT

    AccountEditWidget *MyAccountEditWidget;
    QTabWidget *MyTabWidget;

private slots:
    void widgetAdded(AccountConfigurationWidget *widget);

public:
    explicit AccountConfigurationWidgetTabAdapter(
        AccountEditWidget *accountEditWidget, QTabWidget *tabWidget, QObject *parent = nullptr);
    virtual ~AccountConfigurationWidgetTabAdapter();
};

#endif   // ACCOUNT_CONFIGURATION_WIDGET_TAB_ADAPTER_H
