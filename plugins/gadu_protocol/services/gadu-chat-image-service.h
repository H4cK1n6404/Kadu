/*
 * %kadu copyright begin%
 * Copyright 2012, 2014 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2011, 2012, 2013, 2014 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#ifndef GADU_CHAT_IMAGE_SERVICE
#define GADU_CHAT_IMAGE_SERVICE

#include "protocols/protocol.h"
#include "protocols/services/chat-image.h"

#include "protocols/services/chat-image-service.h"

#include "gadu-exports.h"

class GaduChatService;
class GaduConnection;

/**
 * @addtogroup Gadu
 * @{
 */

/**
 * @class GaduChatImageService
 * @short Service for downloading and uploading avatars in Gadu-Gadu protocol.
 *
 * This service requries proper GaduConnection instance to work. Set it using setConnection() method.
 *
 * This service will only allow for some number of images sent by peers per minute to disable DOS attacks.
 * To manually re-enable receiving images in given minute call resetSendImageRequests().
 */
class GADUAPI GaduChatImageService : public ChatImageService
{
    Q_OBJECT

    static const qint64 RECOMMENDED_MAXIMUM_SIZE = 255 * 1024;

    QMap<ChatImage, QByteArray> ChatImages;

    QPointer<GaduConnection> Connection;
    QPointer<GaduChatService> CurrentChatService;

    bool ReceiveImages;

    friend class GaduProtocolSocketNotifiers;
    void handleEventImageRequest(struct gg_event *e);
    void handleEventImageReply(struct gg_event *e);

    ChatImage chatImageFromSizeCrc32(quint32 size, quint32 crc32) const;

private slots:
    void chatImageKeyReceivedSlot(const QString &id, const ChatImage &chatImage);

public:
    /**
     * @short Create new instance of GaduChatImageService.
     * @param account account bounded to this service
     * @param parent QObject parent
     */
    explicit GaduChatImageService(Account account, QObject *parent = nullptr);
    virtual ~GaduChatImageService();

    /**
     * @short Set connection for this service.
     * @param connection connection for this service
     */
    void setConnection(GaduConnection *connection);

    /**
     * @short Set chat service for this service.
     * @param gaduChatService chat service for this service
     *
     * This service is used to get information about received image keys.
     */
    void setGaduChatService(GaduChatService *gaduChatService);

    /**
     * @short Enable or disable receiving images.
     * @param receiveImages new value of receiveImages property
     *
     * Default value of receiveImages is false. Set it to true to receive images.
     */
    void setReceiveImages(bool receiveImages);

    virtual Error checkImageSize(qint64 size) const;
    virtual ChatImage prepareImageToBeSent(const QByteArray &imageData);
    virtual void requestChatImage(const QString &id, const ChatImage &chatImage);
};

/**
 * @}
 */

#endif   // GADU_CHAT_IMAGE_SERVICE
