/*
 * %kadu copyright begin%
 * Copyright 2016 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include "windows-integration-plugin-modules-factory.h"
#include "windows-integration-plugin-modules-factory.moc"

#include "windows-integration-module.h"

#include <injeqt/injector.h>

WindowsIntegrationPluginModulesFactory::WindowsIntegrationPluginModulesFactory(QObject *parent)
        : PluginModulesFactory{parent}
{
}

WindowsIntegrationPluginModulesFactory::~WindowsIntegrationPluginModulesFactory()
{
}

std::vector<std::unique_ptr<injeqt::module>> WindowsIntegrationPluginModulesFactory::createPluginModules() const
{
    auto modules = std::vector<std::unique_ptr<injeqt::module>>{};
    modules.emplace_back(std::make_unique<WindowsIntegrationModule>());

    return modules;
}
