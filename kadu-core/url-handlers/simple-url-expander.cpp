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

#include <QtXml/QDomDocument>

#include "simple-url-expander.h"

SimpleUrlExpander::SimpleUrlExpander(QRegExp regExp) :
		DomTextRegexpVisitor(regExp)
{
}

SimpleUrlExpander::~SimpleUrlExpander()
{
}

QDomNode SimpleUrlExpander::matchToDomNode(QDomDocument document, QRegExp regExp)
{
	QDomElement linkElement = document.createElement("a");
	QString link = regExp.cap();

	linkElement.setAttribute("href", link);
	linkElement.appendChild(document.createTextNode(link));
	return linkElement;
}