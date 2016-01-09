/*
 * %kadu copyright begin%
 * Copyright 2011 Tomasz Rostanski (rozteck@interia.pl)
 * Copyright 2011 Piotr Galiszewski (piotr.galiszewski@kadu.im)
 * Copyright 2012 Wojciech Treter (juzefwt@gmail.com)
 * Copyright 2012 Piotr Dąbrowski (ultr@ultr.pl)
 * Copyright 2011, 2012, 2013, 2014 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2011, 2012, 2013, 2014, 2015 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * %kadu copyright end%
 *
 * Copyright 2011 Adam "Vertex" Makświej (vertexbz@gmail.com)
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

#include "docking.h"

#include "docking-configuration-provider.h"
#include "docking-menu-action-repository.h"
#include "docking-menu-handler.h"
#include "docking-tooltip-handler.h"
#include "status-notifier-item.h"
#include "status-notifier-item-attention-mode.h"
#include "status-notifier-item-configuration.h"

#include "core/core.h"
#include "core/core.h"
#include "gui/status-icon.h"
#include "gui/widgets/chat-widget/chat-widget-manager.h"
#include "gui/windows/kadu-window.h"
#include "icons/icons-manager.h"
#include "icons/kadu-icon.h"
#include "message/message.h"
#include "message/unread-message-repository.h"
#include "provider/default-provider.h"
#include "status/status-container-manager.h"
#include "activate.h"
#include "attention-service.h"

Docking::Docking(QObject *parent) :
		QObject{parent}
{
}

Docking::~Docking()
{
	if (!Core::instance()->isClosing())
		Core::instance()->kaduWindow()->window()->show();
	Core::instance()->kaduWindow()->setDocked(false);
}

void Docking::setAttentionService(AttentionService *attentionService)
{
	connect(attentionService, SIGNAL(needAttentionChanged(bool)), this, SLOT(needAttentionChanged(bool)));
}

void Docking::setIconsManager(IconsManager *iconsManager)
{
	m_iconsManager = iconsManager;
}

void Docking::setStatusContainerManager(StatusContainerManager *statusContainerManager)
{
	m_statusContainerManager = statusContainerManager;
}

void Docking::init()
{
	m_dockingMenuActionRepository = make_owned<DockingMenuActionRepository>(this);

	auto statusIcon = make_owned<StatusIcon>(m_statusContainerManager, this);
	connect(statusIcon.get(), SIGNAL(iconUpdated(KaduIcon)), this, SLOT(configurationUpdated()));

	connect(Core::instance(), SIGNAL(searchingForTrayPosition(QPoint&)), this, SLOT(searchingForTrayPosition(QPoint&)));

	m_statusNotifierItem = make_owned<StatusNotifierItem>(this);
	connect(m_statusNotifierItem.get(), SIGNAL(activateRequested()), this, SLOT(activateRequested()));
	connect(m_statusNotifierItem.get(), SIGNAL(messageClicked()), this, SIGNAL(messageClicked()));

#ifdef Q_OS_WIN
	m_statusNotifierItem->setIconLoader([](const QString &x){ return QIcon{x}; });
#else
	m_statusNotifierItem->setIconLoader([](const QString &x){ return QIcon::fromTheme(x); });
#endif

	m_dockingConfigurationProvider = make_owned<DockingConfigurationProvider>(this);

	auto dockingMenuHandler = make_owned<DockingMenuHandler>(m_statusNotifierItem->contextMenu(), this);
	dockingMenuHandler->setDockingMenuActionRepository(m_dockingMenuActionRepository.get());
	dockingMenuHandler->setIconsManager(m_iconsManager);
	dockingMenuHandler->setNotificationService(Core::instance()->notificationService());
	dockingMenuHandler->setStatusContainerManager(Core::instance()->statusContainerManager());

	auto dockingTooltipHandler = make_owned<DockingTooltipHandler>(m_statusNotifierItem.get(), this);
	dockingTooltipHandler->setDockingConfigurationProvider(m_dockingConfigurationProvider.get());
	dockingTooltipHandler->setStatusContainerManager(Core::instance()->statusContainerManager());

	connect(m_dockingConfigurationProvider.get(), SIGNAL(updated()), this, SLOT(configurationUpdated()));
	configurationUpdated();

	if (m_dockingConfigurationProvider->configuration().RunDocked)
		Core::instance()->setShowMainWindowOnStart(false);
	Core::instance()->kaduWindow()->setDocked(true);
}

DockingMenuActionRepository * Docking::dockingMenuActionRepository() const
{
	return m_dockingMenuActionRepository.get();
}

void Docking::needAttentionChanged(bool needAttention)
{
	m_statusNotifierItem->setNeedAttention(needAttention);
}

void Docking::openUnreadMessages()
{
	auto message = Core::instance()->unreadMessageRepository()->unreadMessage();
	Core::instance()->chatWidgetManager()->openChat(message.messageChat(), OpenChatActivation::Activate);
}

void Docking::showMessage(QString title, QString message, QSystemTrayIcon::MessageIcon icon, int msecs)
{
	m_statusNotifierItem->showMessage(title, message, icon, msecs);
}

void Docking::activateRequested()
{
	if (Core::instance()->unreadMessageRepository()->hasUnreadMessages())
	{
		openUnreadMessages();
		return;
	}

	auto kaduWindow = Core::instance()->kaduWindow()->window();
	if (kaduWindow->isMinimized() || !kaduWindow->isVisible()
#ifndef Q_OS_WIN
			// NOTE: It won't work as expected on Windows since when you click on tray icon,
			// the tray will become active and any other window will loose focus.
			// See bug #1915.
			|| !_isActiveWindow(kaduWindow)
#endif
			)
		_activateWindow(kaduWindow);
	else
		kaduWindow->hide();
}

void Docking::searchingForTrayPosition(QPoint &point)
{
	point = m_statusNotifierItem->trayPosition();
}

void Docking::configurationUpdated()
{
	auto configuration = StatusNotifierItemConfiguration{};
	configuration.AttentionMode = m_dockingConfigurationProvider->configuration().NewMessageIcon;
	configuration.AttentionIcon = KaduIcon{"protocols/common/message", "128x128"}.fullPath();
	configuration.AttentionMovie = KaduIcon{"protocols/common/message_anim", "16x16"}.fullPath();
	configuration.Icon = Core::instance()->statusContainerManager()->statusIcon().fullPath();
	m_statusNotifierItem->setConfiguration(configuration);
}

#include "moc_docking.cpp"
