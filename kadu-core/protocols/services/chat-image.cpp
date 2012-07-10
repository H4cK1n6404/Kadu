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

#include "chat-image.h"

ChatImage::ChatImage()
{
}

ChatImage::ChatImage(const ChatImage &copyMe)
{
	LocalFileName = copyMe.LocalFileName;
	Content = copyMe.Content;
	Crc32 = copyMe.Crc32;
}

ChatImage & ChatImage::operator = (const ChatImage &copyMe)
{
	LocalFileName = copyMe.LocalFileName;
	Content = copyMe.Content;
	Crc32 = copyMe.Crc32;

	return *this;
}

void ChatImage::setLocalFileName(const QString &localFileName)
{
	LocalFileName = localFileName;
}

QString ChatImage::localFileName() const
{
	return LocalFileName;
}

void ChatImage::setContent(const QByteArray &content)
{
	Content = content;
}

QByteArray ChatImage::content() const
{
	return Content;
}

void ChatImage::setCrc32(quint32 crc32)
{
	Crc32 = crc32;
}

quint32 ChatImage::crc32() const
{
	return Crc32;
}