/*
 * %kadu copyright begin%
 * Copyright 2010 Piotr Galiszewski (piotr.galiszewski@kadu.im)
 * Copyright 2009, 2009 Wojciech Treter (juzefwt@gmail.com)
 * Copyright 2009 Michał Podsiadlik (michal@kadu.net)
 * Copyright 2009 Bartłomiej Zimoń (uzi18@o2.pl)
 * Copyright 2009, 2010, 2011, 2012, 2013, 2014 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * Copyright 2010, 2011, 2012, 2013 Bartosz Brachaczek (b.brachaczek@gmail.com)
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

#include <xmpp/xmpp-im/xmpp_bytestream.h>
#include <filetransfer.h>

#include "file-transfer/file-transfer-direction.h"
#include "file-transfer/file-transfer-status.h"

#include "resource/jabber-resource-pool.h"
#include "jabber-protocol.h"

#include "jabber-file-transfer-handler.h"

JabberFileTransferHandler::JabberFileTransferHandler(::FileTransfer transfer) :
		FileTransferHandler(transfer), JabberTransfer(0), InProgress(false), BytesTransferred(0)
{
}

JabberFileTransferHandler::~JabberFileTransferHandler()
{
	cleanup(transfer().transferStatus());
}

void JabberFileTransferHandler::connectJabberTransfer()
{
	if (!JabberTransfer)
		return;

	connect(JabberTransfer, SIGNAL(accepted()), this, SLOT(fileTransferAccepted()));
	connect(JabberTransfer, SIGNAL(connected()), this, SLOT(fileTransferConnected()));
	connect(JabberTransfer, SIGNAL(readyRead(const QByteArray &)), this, SLOT(fileTransferReadyRead(const QByteArray &)));
	connect(JabberTransfer, SIGNAL(bytesWritten(int)), this, SLOT(fileTransferBytesWritten(int)));
	connect(JabberTransfer, SIGNAL(error(int)), this, SLOT(fileTransferError(int)));
}

void JabberFileTransferHandler::disconnectJabberTransfer()
{
	if (JabberTransfer)
		disconnect(JabberTransfer, 0, this, 0);
}

void JabberFileTransferHandler::setJTransfer(XMPP::FileTransfer *jTransfer)
{
	disconnectJabberTransfer();
	JabberTransfer = jTransfer;
	connectJabberTransfer();
}

void JabberFileTransferHandler::cleanup(FileTransferStatus status)
{
	InProgress = false;

	transfer().setTransferStatus(status);

	if (JabberTransfer)
	{
		JabberTransfer->deleteLater();
		JabberTransfer = nullptr;
	}

	if (Destination)
	{
		Destination->close();
		Destination->deleteLater();
	}

	deleteLater();
}

void JabberFileTransferHandler::updateFileInfo()
{
	if (JabberTransfer)
		transfer().setTransferredSize(BytesTransferred);
	else
		transfer().setTransferredSize(0);

	emit statusChanged();
}

void JabberFileTransferHandler::send()
{
	if (FileTransferDirection::Outgoing != transfer().transferDirection()) // maybe assert here?
		return;

	if (InProgress) // already sending/receiving
		return;

	QFileInfo localFile(transfer().localFileName());
	transfer().setRemoteFileName(localFile.fileName());

	QFileInfo fileInfo(transfer().localFileName());
	transfer().setFileSize(fileInfo.size());

	Account account = transfer().peer().contactAccount();
	if (account.isNull() || transfer().localFileName().isEmpty())
	{
		transfer().setTransferStatus(FileTransferStatus::NotConnected);
		deleteLater();
		return; // TODO: notify
	}

	XMPP::JabberProtocol *jabberProtocol = dynamic_cast<XMPP::JabberProtocol *>(account.protocolHandler());
	if (!jabberProtocol)
	{
		transfer().setTransferStatus(FileTransferStatus::NotConnected);
		deleteLater();
		return;
	}

	if (!jabberProtocol->jabberContactDetails(transfer().peer()))
	{
		transfer().setTransferStatus(FileTransferStatus::NotConnected);
		deleteLater();
		return;
	}

	QString jid = transfer().peer().id();
	// sendFile needs jid with resource so take best from ResourcePool
	PeerJid = XMPP::Jid(jid).withResource(jabberProtocol->resourcePool()->bestResource(jid).name());

	if (!JabberTransfer)
	{
		JabberTransfer = jabberProtocol->xmppClient()->fileTransferManager()->createTransfer();
		connectJabberTransfer();
	}

	JabberAccountDetails *jabberAccountDetails = dynamic_cast<JabberAccountDetails *>(account.details());
	XMPP::Jid proxy;
	if (0 != jabberAccountDetails)
		proxy = jabberAccountDetails->dataTransferProxy();

	if (proxy.isValid())
		JabberTransfer->setProxy(proxy);

	transfer().setTransferStatus(FileTransferStatus::WaitingForAccept);
	InProgress = true;

	JabberTransfer->sendFile(PeerJid, transfer().remoteFileName(), transfer().fileSize(), QString(), XMPP::FTThumbnail());
}

void JabberFileTransferHandler::stop()
{
	if (JabberTransfer)
		JabberTransfer->close();

	cleanup(FileTransferStatus::NotConnected);
}

void JabberFileTransferHandler::accept(QIODevice *destination)
{
	Destination = destination;
	BytesTransferred = 0;

	transfer().setTransferStatus(FileTransferStatus::Transfer);
	transfer().setTransferredSize(BytesTransferred);

	if (FileTransferDirection::Incoming == transfer().transferDirection())
		transfer().setFileSize(JabberTransfer->fileSize());

	JabberTransfer->accept(BytesTransferred);
}

void JabberFileTransferHandler::reject()
{
	if (JabberTransfer)
		JabberTransfer->close();

	deleteLater();
}

void JabberFileTransferHandler::fileTransferAccepted()
{
	transfer().setTransferStatus(FileTransferStatus::WaitingForConnection);
}

void JabberFileTransferHandler::fileTransferConnected()
{
	if (FileTransferDirection::Outgoing == transfer().transferDirection())
	{
		if (LocalFile.isOpen()) // ?? assert
		{
			cleanup(FileTransferStatus::NotConnected);
			return;
		}

		LocalFile.setFileName(transfer().localFileName());
		if (!LocalFile.open(QIODevice::ReadOnly))
		{
			cleanup(FileTransferStatus::NotConnected);
			return;
		}

		BytesTransferred = JabberTransfer->offset();
		if (0 != BytesTransferred && !LocalFile.seek(BytesTransferred))
		{
			cleanup(FileTransferStatus::NotConnected);
			return;
		}

		fileTransferBytesWritten(0);
	}
	// on TypeReceive fileTransferReadyRead will be called automatically

	transfer().setTransferStatus(FileTransferStatus::Transfer);
}

void JabberFileTransferHandler::fileTransferReadyRead(const QByteArray &a)
{
	if (Destination)
		Destination->write(a);

	BytesTransferred += a.size();
	updateFileInfo();

	if (BytesTransferred == JabberTransfer->fileSize())
		cleanup(FileTransferStatus::Finished);
}

void JabberFileTransferHandler::fileTransferBytesWritten(int written)
{
	BytesTransferred += written;
	updateFileInfo();

	if (BytesTransferred == (qlonglong)(transfer().fileSize()))
	{
		cleanup(FileTransferStatus::Finished);
		return;
	}

	if (!JabberTransfer->bsConnection())
	{
		cleanup(FileTransferStatus::NotConnected);
		return;
	}

	int dataSize = JabberTransfer->dataSizeNeeded();
	QByteArray data(dataSize, (char)0);

	int sizeRead = LocalFile.read(data.data(), data.size());
	if (sizeRead < 0)
	{
		cleanup(FileTransferStatus::NotConnected);
		return;
	}

	if (sizeRead < data.size())
		data.resize(sizeRead);

	JabberTransfer->writeFileData(data);
}

FileTransferStatus JabberFileTransferHandler::errorToStatus(int error)
{
	switch (error)
	{
		case XMPP::FileTransfer::ErrReject:
			return FileTransferStatus::Rejected;
			break;
		case XMPP::FileTransfer::ErrNeg:
		case XMPP::FileTransfer::ErrConnect:
		case XMPP::FileTransfer::ErrStream:
		default:
			return FileTransferStatus::NotConnected;
			break;
	}
}

void JabberFileTransferHandler::fileTransferError(int error)
{
	cleanup(errorToStatus(error));
}

#include "moc_jabber-file-transfer-handler.cpp"
