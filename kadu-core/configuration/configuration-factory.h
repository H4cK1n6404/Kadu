/*
 * %kadu copyright begin%
 * Copyright 2014 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include "configuration/configuration-path-provider.h"
#include "configuration/configuration.h"
#include "exports.h"
#include "misc/memory.h"

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <injeqt/injeqt.h>

class Configuration;
class VersionService;

class KADUAPI ConfigurationFactory final : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit ConfigurationFactory(QObject *parent = nullptr);
    virtual ~ConfigurationFactory();

    Q_INVOKABLE Configuration *createConfiguration() const;

private:
    QPointer<ConfigurationPathProvider> m_configurationPathProvider;
    QPointer<VersionService> m_versionService;

    not_owned_qptr<Configuration> readConfiguration() const;
    not_owned_qptr<Configuration> createEmptyConfiguration() const;
    bool isConfigurationPathUsable() const;

private slots:
    INJEQT_SET void setConfigurationPathProvider(ConfigurationPathProvider *configurationPathProvider);
    INJEQT_SET void setVersionService(VersionService *versionService);
};
