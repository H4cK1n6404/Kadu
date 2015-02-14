/*
 * %kadu copyright begin%
 * Copyright 2008, 2009, 2010, 2010, 2011 Piotr Galiszewski (piotr.galiszewski@kadu.im)
 * Copyright 2009, 2010, 2012 Wojciech Treter (juzefwt@gmail.com)
 * Copyright 2008, 2009 Tomasz Rostański (rozteck@interia.pl)
 * Copyright 2008, 2009 Michał Podsiadlik (michal@kadu.net)
 * Copyright 2005, 2006 Adrian Smarzewski (adrian@kadu.net)
 * Copyright 2005 Paweł Płuciennik (pawel_p@kadu.net)
 * Copyright 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * Copyright 2010, 2011, 2013 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2006, 2007 Dawid Stawiarski (neeo@kadu.net)
 * Copyright 2005, 2006 Marcin Ślusarz (joi@kadu.net)
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

#include "configuration/configuration.h"
#include "configuration/deprecated-configuration-api.h"
#include "core/application.h"
#include "gui/actions/action-description.h"
#include "gui/actions/action.h"
#include "gui/menu/menu-inventory.h"

#include "sound-manager.h"

#include "sound-actions.h"

SoundActions::SoundActions(QObject *parent) :
		QObject{parent}
{
	m_muteActionDescription = new ActionDescription{this,
		ActionDescription::TypeGlobal, "muteSoundsAction",
		this, SLOT(muteActionActivated(QAction *, bool)),
		KaduIcon("audio-volume-high"), tr("Play Sounds"), true
	};

	MenuInventory::instance()
		->menu("main")
		->addAction(m_muteActionDescription, KaduMenu::SectionMiscTools, 7)
		->update();
}

SoundActions::~SoundActions()
{
	MenuInventory::instance()
		->menu("main")
		->removeAction(m_muteActionDescription)
		->update();
}

void SoundActions::setSoundManager(SoundManager *soundManager)
{
	m_soundManager = soundManager;

	setMuteActionState();
	connect(m_muteActionDescription, SIGNAL(actionCreated(Action *)), this, SLOT(setMuteActionState()));
}

void SoundActions::setMuteActionState()
{
	for (auto action : m_muteActionDescription->actions())
		action->setChecked(!m_soundManager->isMuted());
}

void SoundActions::muteActionActivated(QAction  *action, bool toggled)
{
	Q_UNUSED(action)

	m_soundManager->setMute(!toggled);
	setMuteActionState();

	Application::instance()->configuration()->deprecatedApi()->writeEntry("Sounds", "PlaySound", toggled);
}

void SoundActions::configurationUpdated()
{
	m_soundManager->setMute(!Application::instance()->configuration()->deprecatedApi()->readBoolEntry("Sounds", "PlaySound"));
	setMuteActionState();
}

#include "moc_sound-actions.cpp"