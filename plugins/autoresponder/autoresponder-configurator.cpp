/*
 * %kadu copyright begin%
 * Copyright 2014 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2012, 2013, 2014 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include <QtCore/QCoreApplication>

#include "configuration/configuration.h"
#include "configuration/deprecated-configuration-api.h"
#include "core/core.h"

#include "autoresponder-configuration.h"
#include "autoresponder-message-filter.h"

#include "autoresponder-configurator.h"

void AutoresponderConfigurator::setAutoresponderMessageFilter(AutoresponderMessageFilter *autoresponderMessageFilter)
{
	ConfigurableAutoresponder = autoresponderMessageFilter;

	createDefaultConfiguration();
	configurationUpdated();
}

void AutoresponderConfigurator::createDefaultConfiguration()
{
	Core::instance()->configuration()->deprecatedApi()->addVariable("Autoresponder", "Autotext", QCoreApplication::translate("AutoresponderConfigurator", "I am busy."));
	Core::instance()->configuration()->deprecatedApi()->addVariable("Autoresponder", "OnlyFirstTime", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Autoresponder", "RespondConf", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Autoresponder", "StatusAvailable", false);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Autoresponder", "StatusBusy", true);
	Core::instance()->configuration()->deprecatedApi()->addVariable("Autoresponder", "StatusInvisible", false);
}

void AutoresponderConfigurator::configurationUpdated()
{
	if (!ConfigurableAutoresponder)
		return;

	AutoresponderConfiguration configuration;
	configuration.setAutoRespondText(Core::instance()->configuration()->deprecatedApi()->readEntry("Autoresponder", "Autotext"));
	configuration.setRespondConferences(Core::instance()->configuration()->deprecatedApi()->readBoolEntry("Autoresponder", "RespondConf"));
	configuration.setRespondOnlyFirst(Core::instance()->configuration()->deprecatedApi()->readBoolEntry("Autoresponder", "OnlyFirstTime"));
	configuration.setStatusAvailable(Core::instance()->configuration()->deprecatedApi()->readBoolEntry("Autoresponder", "StatusAvailable"));
	configuration.setStatusBusy(Core::instance()->configuration()->deprecatedApi()->readBoolEntry("Autoresponder", "StatusBusy"));
	configuration.setStatusInvisible(Core::instance()->configuration()->deprecatedApi()->readBoolEntry("Autoresponder", "StatusInvisible"));

	ConfigurableAutoresponder->setConfiguration(configuration);
}
