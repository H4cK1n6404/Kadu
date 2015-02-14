/*
 * %kadu copyright begin%
 * Copyright 2010, 2010, 2011 Piotr Galiszewski (piotr.galiszewski@kadu.im)
 * Copyright 2011 Piotr Dąbrowski (ultr@ultr.pl)
 * Copyright 2010, 2011, 2013 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * Copyright 2010, 2011, 2013 Bartosz Brachaczek (b.brachaczek@gmail.com)
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

#include "sound-configuration-widget.h"

#include "sound-manager.h"

#include "configuration/configuration.h"
#include "configuration/deprecated-configuration-api.h"
#include "core/application.h"
#include "gui/widgets/configuration/notify-group-box.h"
#include "gui/widgets/select-file.h"
#include "icons/kadu-icon.h"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>

SoundConfigurationWidget::SoundConfigurationWidget(SoundManager *manager, QWidget *parent) :
		NotifierConfigurationWidget{parent},
		m_manager{manager}
{
	auto testButton = new QPushButton{KaduIcon{"external_modules/mediaplayer-media-playback-play"}.icon(), QString{}, this};
	connect(testButton, SIGNAL(clicked()), this, SLOT(test()));

	m_soundSelectFile = new SelectFile{"audio", this};
	connect(m_soundSelectFile, SIGNAL(fileChanged()), this, SIGNAL(soundFileEdited()));

	auto layout = new QHBoxLayout{this};
	layout->insertSpacing(0, 20);
	layout->addWidget(testButton);
	layout->addWidget(m_soundSelectFile);

	static_cast<NotifyGroupBox *>(parent)->addWidget(this);
}

SoundConfigurationWidget::~SoundConfigurationWidget()
{
}

void SoundConfigurationWidget::test()
{
	m_manager->playFile(m_soundSelectFile->file(), true);
}

void SoundConfigurationWidget::saveNotifyConfigurations()
{
	if (!m_currentNotifyEvent.isEmpty())
		m_soundFiles[m_currentNotifyEvent] = m_soundSelectFile->file();

	for (auto it = m_soundFiles.constBegin(), end = m_soundFiles.constEnd(); it != end; ++it)
		Application::instance()->configuration()->deprecatedApi()->writeEntry("Sounds", it.key() + "_sound", it.value());
}

void SoundConfigurationWidget::switchToEvent(const QString &event)
{
	if (!m_currentNotifyEvent.isEmpty())
		m_soundFiles[m_currentNotifyEvent] = m_soundSelectFile->file();
	m_currentNotifyEvent = event;

	if (m_soundFiles.contains(event))
		m_soundSelectFile->setFile(m_soundFiles[event]);
	else
		m_soundSelectFile->setFile(Application::instance()->configuration()->deprecatedApi()->readEntry("Sounds", event + "_sound"));
}

void SoundConfigurationWidget::themeChanged(int index)
{
	if (index == 0)
		return;

	//refresh soundFiles
	for (auto it = m_soundFiles.begin(), end = m_soundFiles.end(); it != end; ++it)
	{
		it.value() = Application::instance()->configuration()->deprecatedApi()->readEntry("Sounds", it.key() + "_sound");
		if (it.key() == m_currentNotifyEvent)
			m_soundSelectFile->setFile(it.value());
	}
}

#include "moc_sound-configuration-widget.cpp"