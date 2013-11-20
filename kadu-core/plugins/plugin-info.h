/*
 * %kadu copyright begin%
 * Copyright 2008, 2011 Piotr Galiszewski (piotr.galiszewski@kadu.im)
 * Copyright 2009, 2012 Wojciech Treter (juzefwt@gmail.com)
 * Copyright 2003, 2004 Adrian Smarzewski (adrian@kadu.net)
 * Copyright 2002, 2003, 2004 Tomasz Chiliński (chilek@chilan.com)
 * Copyright 2008, 2010, 2011 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * Copyright 2010, 2011 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2008 Dawid Stawiarski (neeo@kadu.net)
 * Copyright 2004, 2005, 2006, 2007 Marcin Ślusarz (joi@kadu.net)
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

#include <QtCore/QString>
#include <QtCore/QStringList>

/**
 * @addtogroup Plugins
 * @{
 */

/**
 * @class PluginInfo
 * @author Rafał 'Vogel' Malinowski
 * @short Information about plugin loaded from .desc files.
 * @todo Rewrite to use .desktop files
 *
 * This class contains information about an installed plugin. Every installed plugin has
 * associated .desc file that contains info like plugin author, version ordependencies.
 * New instances of this class are created with existing file name as paremeters. Otherwise,
 * object will contain invalid (empty) data.
 */
class PluginInfo
{

public:
	PluginInfo(
			QString name, QString displayName, QString category, QString type, QString description, QString author, QString version,
			QStringList dependencies, QStringList conflicts, QStringList provides, QStringList replaces, bool loadByDefault);
	~PluginInfo();


	QString name() const;

	QString displayName() const;

	QString category() const;

	/**
	 * @author Rafał 'Vogel' Malinowski
	 * @short Returns type of plugin.
	 * @return type of plugin
	 *
	 * Returns type of plugin. In Kadu 0.10.0 only one type of plugin is supported:
	 * Protocol plugin. All plugins that are not protocols should have this field
	 * empty.
	 *
	 * Plugins with type Protocol are activated before all other plugins (see
	 * PluginsManager::activateProtocolPlugins()).
	 */
	QString type() const;

	/**
	 * @author Rafał 'Vogel' Malinowski
	 * @short Returns description of plugin.
	 * @return description of plugin
	 *
	 * Returns description of plugin. If no description in current system language is available
	 * default one will be used.
	 */
	QString description() const;

	/**
	 * @author Rafał 'Vogel' Malinowski
	 * @short Returns name of plugin's author.
	 * @return name of plugin's author
	 *
	 * Returns name of plugin's author.
	 */
	QString author() const;

	/**
	 * @author Rafał 'Vogel' Malinowski
	 * @short Returns plugin's version.
	 * @return plugin's version
	 *
	 * Returns plugin's version.
	 */
	QString version() const;

	/**
	 * @author Rafał 'Vogel' Malinowski
	 * @short Returns list of dependencies of plugin.
	 * @return list of dependencies of plugin
	 *
	 * Returns list of dependencies of plugin. This list contains names of plugins that have to be
	 * activated before this plugin. If any of plugins in this list is not available or could not
	 * be activated, this plugin wont get activated.
	 */
	QStringList dependencies() const;

	/**
	 * @author Rafał 'Vogel' Malinowski
	 * @short Returns list of conflicts of plugin.
	 * @return list of conflicts of plugin
	 *
	 * Returns list of conflicts of plugin. This list contains names of plugins and "provides" strings that
	 * that conflicts with this plugin. If any plugin from this list is active or "provides" string that
	 * is in this list, this plugin can not be activated.
	 */
	QStringList conflicts() const;

	/**
	 * @author Rafał 'Vogel' Malinowski
	 * @short Returns list of featured of plugin.
	 * @return list of conflicts of plugin
	 * @todo consider remove and replacing with Type
	 *
	 * Returns list of features of this plugin. Two active plugins can not have the same string in their
	 * "provides" lists.
	 */
	QStringList provides() const;

	/**
	 * @author Rafał 'Vogel' Malinowski
	 * @short Returns list of plugins replaced by this one.
	 * @return list of plugins replaced by this one
	 *
	 * Returns list of plugins replaced by this one. If plugin with name from this list was active in previous
	 * run of Kadu and is no longer avilable, this plugin will be activated instead. This allows us to
	 * change name of plugins and to create more generic plugins from less generic.
	 */
	QStringList replaces() const;

	/**
	 * @author Rafał 'Vogel' Malinowski
	 * @short Returns load by default value for this plugin.
	 * @return load by default value for this plugin
	 *
	 * Returns load by default value for this plugin. If plugin has load by default set
	 * to true and it is new (was not known in previous run of Kadu) it will be activated
	 * automatically on start. This option should be only set for plugins that are very
	 * important, like new protocols plugns or plugins that imports data from older Kadu
	 * version.
	 */
	bool loadByDefault() const;

private:
	QString m_name;
	QString m_displayName;
	QString m_category;
	QString m_type;
	QString m_description;
	QString m_author;
	QString m_version;
	QStringList m_dependencies;
	QStringList m_conflicts;
	QStringList m_provides;
	QStringList m_replaces;
	bool m_loadByDefault;

};

/**
 * @}
 */
