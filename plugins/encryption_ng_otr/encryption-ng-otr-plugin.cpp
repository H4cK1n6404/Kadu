/*
 * %kadu copyright begin%
 * Copyright 2012 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include "core/core.h"
#include "services/message-filter-service.h"

#include "encryption-ng-otr-message-filter.h"
#include "encryption-ng-otr-user-state-service.h"

#include "encryption-ng-otr-plugin.h"

EngryptionNgOtrPlugin::~EngryptionNgOtrPlugin()
{
}

void EngryptionNgOtrPlugin::registerOtrUserStateService()
{
	OtrUserStateService.reset(new EncryptionNgOtrUserStateService(this));
}

void EngryptionNgOtrPlugin::unregisterOtrUserStateService()
{
	OtrUserStateService.reset();
}

void EngryptionNgOtrPlugin::registerOtrMessageFilter()
{
	OtrMessageFilter.reset(new EncryptionNgOtrMessageFilter(this));

	Core::instance()->messageFilterService()->registerMessageFilter(OtrMessageFilter.data());
}

void EngryptionNgOtrPlugin::unregisterOtrMessageFilter()
{
	Core::instance()->messageFilterService()->unregisterMessageFilter(OtrMessageFilter.data());

	OtrMessageFilter.reset();
}

int EngryptionNgOtrPlugin::init(bool firstLoad)
{
	Q_UNUSED(firstLoad);

	registerOtrUserStateService();
	registerOtrMessageFilter();

	return 0;
}

void EngryptionNgOtrPlugin::done()
{
	unregisterOtrUserStateService();
	unregisterOtrMessageFilter();
}

Q_EXPORT_PLUGIN2(encryption_ng_otr, EngryptionNgOtrPlugin)
