/*
 * %kadu copyright begin%
 * Copyright 2013 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include <map>
#include <memory>
#include <set>
#include <QtCore/QObject>
#include <QtCore/QWeakPointer>

#include "plugins/dependency-graph/plugin-dependency-graph.h"
#include "exports.h"

class PluginInfo;
class PluginRepository;

class KADUAPI PluginDependencyGraphBuilder : public QObject
{
	Q_OBJECT

public:
	explicit PluginDependencyGraphBuilder(QObject *parent = nullptr);
	virtual ~PluginDependencyGraphBuilder();

	void setPluginRepository(PluginRepository *pluginRepository);

	std::unique_ptr<PluginDependencyGraph> buildGraph() const;

private:
	QWeakPointer<PluginRepository> m_pluginRepository;

	std::set<QString> getPluginNames() const;
	std::map<QString, std::unique_ptr<PluginDependencyGraphNode>> createNodes(std::set<QString> pluginNames) const;
	void connectNodes(std::map<QString, std::unique_ptr<PluginDependencyGraphNode>> &nodes) const;
	std::vector<std::unique_ptr<PluginDependencyGraphNode>> convertToVector(std::map<QString, std::unique_ptr<PluginDependencyGraphNode>> &nodes) const;

};
