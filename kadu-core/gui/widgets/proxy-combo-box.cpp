/*
 * %kadu copyright begin%
 * Copyright 2011, 2013, 2014 Bartosz Brachaczek (b.brachaczek@gmail.com)
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

#include <QtWidgets/QAction>

#include "core/core.h"
#include "core/injected-factory.h"
#include "gui/windows/proxy-edit-window-service.h"
#include "model/model-chain.h"
#include "model/roles.h"
#include "network/proxy/model/network-proxy-model.h"
#include "network/proxy/model/network-proxy-proxy-model.h"

#include "proxy-combo-box.h"

#define DEFAULT_PROXY_INDEX 1

ProxyComboBox::ProxyComboBox(QWidget *parent) :
		ActionsComboBox(parent), DefaultProxyAction(0)
{
	addBeforeAction(new QAction(tr(" - No proxy - "), this));

	ModelChain *chain = new ModelChain(this);
	Model = Core::instance()->injectedFactory()->makeInjected<NetworkProxyModel>(chain);
	chain->setBaseModel(Model);
	chain->addProxyModel(new NetworkProxyProxyModel(this));
	setUpModel(NetworkProxyRole, chain);

	EditProxyAction = new QAction(tr("Edit proxy configuration..."), this);
	QFont editProxyActionFont = EditProxyAction->font();
	editProxyActionFont.setItalic(true);
	EditProxyAction->setFont(editProxyActionFont);
	EditProxyAction->setData(true);
	connect(EditProxyAction, SIGNAL(triggered()), this, SLOT(editProxy()));

	addAfterAction(EditProxyAction);
}

ProxyComboBox::~ProxyComboBox()
{
}

void ProxyComboBox::enableDefaultProxyAction()
{
	DefaultProxyAction = new QAction(tr(" - Use Default Proxy - "), this);
	DefaultProxyAction->setFont(QFont());
	addBeforeAction(DefaultProxyAction);
}

void ProxyComboBox::selectDefaultProxy()
{
	if (DefaultProxyAction)
		setCurrentIndex(DEFAULT_PROXY_INDEX);
}

bool ProxyComboBox::isDefaultProxySelected()
{
	if (DefaultProxyAction)
		return DEFAULT_PROXY_INDEX == currentIndex();
	else
		return false;
}

void ProxyComboBox::setCurrentProxy(const NetworkProxy &networkProxy)
{
	setCurrentValue(networkProxy);
}

NetworkProxy ProxyComboBox::currentProxy()
{
	return currentValue().value<NetworkProxy>();
}

void ProxyComboBox::editProxy()
{
	Core::instance()->proxyEditWindowService()->show();
}

#include "moc_proxy-combo-box.cpp"
