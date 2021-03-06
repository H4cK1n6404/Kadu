/*
 * %kadu copyright begin%
 * Copyright 2009, 2011 Piotr Galiszewski (piotr.galiszewski@kadu.im)
 * Copyright 2010 Tomasz Rostański (rozteck@interia.pl)
 * Copyright 2011 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2010, 2011, 2013, 2014 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#ifndef COMPOSITING_AWARE_OBJECT
#define COMPOSITING_AWARE_OBJECT

#include "exports.h"

#include <QtCore/QList>

class KADUAPI CompositingAwareObject
{
    static bool CompositingEnabled;
    static QList<CompositingAwareObject *> Objects;

protected:
    virtual void compositingEnabled() = 0;
    virtual void compositingDisabled() = 0;

public:
    static void compositingStateChanged();

    CompositingAwareObject();
    virtual ~CompositingAwareObject();

    void triggerCompositingStateChanged();

    bool isCompositingEnabled() const;
};

#endif   // COMPOSITING_AWARE_OBJECT
