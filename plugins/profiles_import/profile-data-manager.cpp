/*
 * %kadu copyright begin%
 * Copyright 2011 Piotr Galiszewski (piotr.galiszewski@kadu.im)
 * Copyright 2011 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include <QtCore/QVector>

#include "configuration/xml-configuration-file.h"
#include "kadu-application.h"

#include "profile-data-manager.h"

QDomElement ProfileDataManager::getProfilesNode()
{
	QDomElement deprecated = KaduApplication::instance()->configurationApi()->getNode("Deprecated", XmlConfigFile::ModeFind);
	if (deprecated.isNull())
		return QDomElement();

	QDomElement configFile;
	QVector<QDomElement> configFiles = KaduApplication::instance()->configurationApi()->getNodes(deprecated, "ConfigFile");

	foreach (const QDomElement &cf, configFiles)
		if (cf.attribute("name").endsWith("kadu.conf"))
		{
			configFile = cf;
			break;
		}

	if (configFile.isNull())
		return QDomElement();

	return KaduApplication::instance()->configurationApi()->getNamedNode(configFile, "Group", "Profiles");
}

QList<ProfileData> ProfileDataManager::readProfileData()
{
	QList<ProfileData> result;

	QDomElement groupProfiles = getProfilesNode();
	if (groupProfiles.isNull())
		return result;

	QVector<QDomElement> profiles = KaduApplication::instance()->configurationApi()->getNodes(groupProfiles, "Profile");
	foreach (const QDomElement &profile, profiles)
	{
		if (profile.attribute("imported") != "yes")
		{
			ProfileData newProfile;
			newProfile.Name = profile.attribute("name");
			newProfile.Path = profile.attribute("directory");
			result.append(newProfile);
		}
	}

	return result;
}

void ProfileDataManager::markImported(const QString &name)
{
	QDomElement groupProfiles = getProfilesNode();
	if (groupProfiles.isNull())
		return;

	QDomElement profile = KaduApplication::instance()->configurationApi()->getNamedNode(groupProfiles, "Profile", name);
	profile.setAttribute("imported", "yes");
}
