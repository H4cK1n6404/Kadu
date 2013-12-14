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

#include "misc/algorithm.h"

#include <deque>
#include <map>
#include <set>
#include <utility>

template<typename GraphType, typename SuccessorTypeTag>
class GraphCycleFinder
{
	using NodePointer = typename GraphType::NodePointer;

public:
	explicit GraphCycleFinder(const GraphType &graph)
	{
		for (auto &node : graph.nodes())
			if (!isVisited(node.get()))
				strongConnect(node.get());
	}

	std::set<NodePointer> result() const
	{
		return m_result;
	}

private:
	std::map<NodePointer, std::pair<int, int>> m_indexes;
	std::deque<NodePointer> m_currnentPath;
	std::set<NodePointer> m_result;
	int m_index;

	bool isVisited(NodePointer node) const
	{
		return m_indexes.count(node) > 0;
	}

	bool isOnCurrentPath(NodePointer node) const
	{
		return contains(m_currnentPath, node);
	}

	bool isRootNode(NodePointer node) const
	{
		return m_indexes.at(node).first == m_indexes.at(node).second;
	}

	std::vector<NodePointer> getCurrentCycle(NodePointer node)
	{
		auto result = std::vector<NodePointer>{};
		while (!m_currnentPath.empty())
		{
			auto stackNode = m_currnentPath.back();

			if (m_indexes.at(node).second == m_indexes.at(stackNode).second)
			{
				m_currnentPath.pop_back();
				result.push_back(stackNode);
			}
			else
				return result;
		}

		return result;
	}

	void strongConnect(NodePointer node)
	{
		m_indexes.insert({node, std::make_pair(m_index, m_index)});
		m_index++;
		m_currnentPath.push_back(node);

		for (auto successor : node->template successors<SuccessorTypeTag>())
		{
			if (!isVisited(successor))
			{
				strongConnect(successor);
				m_indexes.at(node).second = std::min(m_indexes.at(node).second, m_indexes.at(successor).second);
			}
			else if (isOnCurrentPath(successor))
				m_indexes.at(node).second = std::min(m_indexes.at(node).second, m_indexes.at(successor).first);
		}

		if (isRootNode(node))
		{
			auto cycle = getCurrentCycle(node);
			if (cycle.size() > 1)
				std::copy(cycle.begin(), cycle.end(), std::inserter(m_result, m_result.end()));
		}
	}

};

template<typename SuccessorTypeTag, typename GraphType>
std::set<typename GraphType::NodePointer> graph_find_cycles(const GraphType &graph)
{
	auto cycleFinder = GraphCycleFinder<GraphType, SuccessorTypeTag>{graph};
	return cycleFinder.result();
}