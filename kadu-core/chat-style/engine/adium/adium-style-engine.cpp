/*
 * %kadu copyright begin%
 * Copyright 2014 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include "adium-style-engine.h"

#include "chat-style/chat-style.h"
#include "chat-style/engine/adium/adium-style-renderer-factory.h"
#include "chat-style/engine/adium/adium-style.h"
#include "core/injected-factory.h"
#include "misc/memory.h"
#include "misc/paths-provider.h"

#include <QtCore/QDir>

AdiumStyleEngine::AdiumStyleEngine(QObject *parent) :
		QObject{parent}
{
}

AdiumStyleEngine::~AdiumStyleEngine()
{
}

void AdiumStyleEngine::setInjectedFactory(InjectedFactory *injectedFactory)
{
	m_injectedFactory = injectedFactory;
}

void AdiumStyleEngine::setPathsProvider(PathsProvider *pathsProvider)
{
	m_pathsProvider = pathsProvider;
}

QString AdiumStyleEngine::isStyleValid(QString stylePath)
{
	return AdiumStyle::isStyleValid(stylePath) ? QDir(stylePath).dirName() : QString();
}

bool AdiumStyleEngine::styleUsesTransparencyByDefault(QString styleName)
{
	auto style = AdiumStyle{styleName};
	return style.defaultBackgroundIsTransparent();
}

QString AdiumStyleEngine::defaultVariant(const QString &styleName)
{
	auto style = AdiumStyle{styleName};
	return style.defaultVariant();
}

QStringList AdiumStyleEngine::styleVariants(QString styleName)
{
	QDir dir;
	QString styleBaseHref = m_pathsProvider->profilePath() + QLatin1String("syntax/chat/") + styleName + QLatin1String("/Contents/Resources/Variants/");
	if (!dir.exists(styleBaseHref))
		styleBaseHref = m_pathsProvider->dataPath() + QLatin1String("syntax/chat/") + styleName + QLatin1String("/Contents/Resources/Variants/");
	dir.setPath(styleBaseHref);
	dir.setNameFilters(QStringList("*.css"));
	return dir.entryList();
}

std::unique_ptr<ChatStyleRendererFactory> AdiumStyleEngine::createRendererFactory(const ChatStyle &chatStyle)
{
	auto style = std::make_shared<AdiumStyle>(chatStyle.name());
	style->setCurrentVariant(chatStyle.variant());

	return m_injectedFactory->makeUnique<AdiumStyleRendererFactory>(style);
}

#include "moc_adium-style-engine.cpp"
