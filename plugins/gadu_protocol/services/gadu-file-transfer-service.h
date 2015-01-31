/*
 * %kadu copyright begin%
 * Copyright 2008, 2010, 2011 Piotr Galiszewski (piotr.galiszewski@kadu.im)
 * Copyright 2009 Wojciech Treter (juzefwt@gmail.com)
 * Copyright 2008 Michał Podsiadlik (michal@kadu.net)
 * Copyright 2009 Bartłomiej Zimoń (uzi18@o2.pl)
 * Copyright 2007, 2008, 2009, 2010, 2011 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * Copyright 2007, 2008 Dawid Stawiarski (neeo@kadu.net)
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

#ifndef GADU_FILE_TRANSFER_SERVICE_H
#define GADU_FILE_TRANSFER_SERVICE_H

#include <libgadu.h>

#include "protocols/protocol.h"
#include "protocols/services/file-transfer-service.h"

class GaduFileTransferHandler;
class GaduProtocol;

class GaduFileTransferService : public FileTransferService
{
	Q_OBJECT

	GaduProtocol *Protocol;

public:
	explicit GaduFileTransferService(GaduProtocol *protocol);
	virtual ~GaduFileTransferService();

	virtual FileTransferHandler * createFileTransferHandler(FileTransfer fileTransfer);

	void fileTransferReceived(Contact peer, QString downloadId, QString fileName);

};

#endif // GADU_FILE_TRANSFER_SERVICE_H

// kate: indent-mode cstyle; replace-tabs off; tab-width 4;
