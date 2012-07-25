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

#include <QtCore/QFileInfo>
#include <QtGui/QTextBlock>
#include <QtGui/QTextDocument>

#include "formatted-string/formatted-message.h"
#include "services/image-storage-service.h"

#include "formatted-string-factory.h"

void FormattedStringFactory::setImageStorageService(ImageStorageService *imageStorageService)
{
	CurrentImageStorageService = imageStorageService;
}

FormattedMessage FormattedStringFactory::fromPlainText(const QString& plainText)
{
	FormattedMessage result;
	if (!plainText.isEmpty())
		result.append(FormattedMessagePart(plainText, false, false, false, QColor()));

	return result;
}

FormattedMessagePart FormattedStringFactory::partFromQTextCharFormat(const QTextCharFormat &textCharFormat, const QString &text)
{
	if (text.isEmpty())
		return FormattedMessagePart();
	else
		return FormattedMessagePart(text, textCharFormat.font().bold(), textCharFormat.font().italic(), textCharFormat.font().underline(), textCharFormat.foreground().color());
}

FormattedMessagePart FormattedStringFactory::partFromQTextImageFormat(const QTextImageFormat& textImageFormat)
{
	QString filePath = textImageFormat.name();
	QFileInfo fileInfo(filePath);

	if (!fileInfo.isAbsolute() || !fileInfo.exists() || !fileInfo.isFile())
		return FormattedMessagePart();

	if (CurrentImageStorageService)
		filePath = CurrentImageStorageService.data()->storeImage(filePath);

	return FormattedMessagePart(filePath);
}

FormattedMessagePart FormattedStringFactory::partFromQTextFragment(const QTextFragment &textFragment, bool prependNewLine)
{
	if (!textFragment.isValid())
		return FormattedMessagePart();

	QTextCharFormat format = textFragment.charFormat();
	if (!format.isImageFormat())
		return partFromQTextCharFormat(format, prependNewLine ? '\n' + textFragment.text() : textFragment.text());
	else
		return partFromQTextImageFormat(format.toImageFormat());
}

QList<FormattedMessagePart> FormattedStringFactory::partsFromQTextBlock(const QTextBlock &textBlock, bool firstBlock)
{
	QList<FormattedMessagePart> result;

	bool firstFragment = true;
	for (QTextBlock::iterator it = textBlock.begin(); !it.atEnd(); ++it)
	{
		FormattedMessagePart part = partFromQTextFragment(it.fragment(), !firstBlock && firstFragment);
		if (!part.isEmpty())
		{
			result.append(part);
			firstFragment = false;
		}
	}

	return result;
}

FormattedMessage FormattedStringFactory::fromHTML(const QString &html)
{
	FormattedMessage result;

	QTextDocument document;
	document.setHtml(html);

	bool firstBlock = true;

	QTextBlock block = document.firstBlock();
	while (block.isValid())
	{
		QList<FormattedMessagePart> parts = partsFromQTextBlock(block, firstBlock);
		foreach (const FormattedMessagePart &part, parts)
			result.append(part);

		block = block.next();
		firstBlock = false;
	}

	return result;
}