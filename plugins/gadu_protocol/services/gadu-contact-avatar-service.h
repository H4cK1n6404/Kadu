/*
 * %kadu copyright begin%
 * Copyright 2017 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include "avatars/contact-avatar-service.h"

#include <libgadu.h>

struct gg_event_user_data_attr;

class GaduContactAvatarService : public ContactAvatarService
{
    Q_OBJECT

public:
    explicit GaduContactAvatarService(Account account, QObject *parent = nullptr);
    virtual ~GaduContactAvatarService();

    virtual void download(const ContactAvatarId &id) override;

    void handleAvatarData(const uin_t uin, const struct gg_event_user_data_attr *const avatarData);
};
