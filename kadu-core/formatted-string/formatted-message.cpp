/*
 * %kadu copyright begin%
 * Copyright 2011 Tomasz Rostanski (rozteck@interia.pl)
 * Copyright 2008, 2009, 2010, 2011 Piotr Galiszewski (piotr.galiszewski@kadu.im)
 * Copyright 2009 Wojciech Treter (juzefwt@gmail.com)
 * Copyright 2008, 2009, 2010, 2011 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * Copyright 2010, 2011 Bartosz Brachaczek (b.brachaczek@gmail.com)
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

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QRegExp>
#include <QtGui/QTextBlock>
#include <QtGui/QTextDocument>

#include "icons/icons-manager.h"
#include "protocols/services/chat-image-service.h"
#include "services/image-storage-service.h"
#include "html_document.h"

#include "formatted-message.h"

FormattedMessage::FormattedMessage()
{
}

FormattedMessage::FormattedMessage(const FormattedMessage &copyMe)
{
	Parts = copyMe.Parts;
}

FormattedMessage & FormattedMessage::operator = (const FormattedMessage &copyMe)
{
	Parts = copyMe.Parts;
	return *this;
}

FormattedMessage::~FormattedMessage()
{
}

const QVector<FormattedMessagePart> & FormattedMessage::parts() const
{
	return Parts;
}

void FormattedMessage::append(const FormattedMessagePart &part)
{
	Parts.append(part);
}

bool FormattedMessage::isEmpty() const
{
	foreach (const FormattedMessagePart &part, Parts)
		if (!part.isEmpty())
			return false;

	return true;
}

QString FormattedMessage::toPlain() const
{
	QString result;
	foreach (const FormattedMessagePart &part, Parts)
		result += part.content();

	result.replace(QChar::LineSeparator, "\n");
	return result;
}

QString FormattedMessage::toHtml() const
{
	QString result;
	foreach (const FormattedMessagePart &part, Parts)
		result += part.toHtml();

	return result;
}