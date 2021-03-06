/*
 * %kadu copyright begin%
 * Copyright 2011 Piotr Dąbrowski (ultr@ultr.pl)
 * Copyright 2011, 2012, 2014 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2011, 2013, 2014 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include <QtWidgets/QWidget>

#include "misc/misc.h"
#include "os/generic/desktop-aware-object-helper.h"

#include "desktop-aware-object.h"

QList<DesktopAwareObject *> DesktopAwareObject::Objects;

DesktopAwareObjectHelper *DesktopAwareObject::Helper = 0;

DesktopAwareObject::DesktopAwareObject(QWidget *widget) : Widget(widget)
{
    Objects.append(this);
    if (!Helper)
        Helper = new DesktopAwareObjectHelper();
}

DesktopAwareObject::~DesktopAwareObject()
{
    Objects.removeAll(this);
}

/**
 * @short Call this method to call resolutionChanged in each DesktopAwareObject object.
 *
 * Calling this method results in calling resolutionChanged in each DesktopAwareObject
 * in system.
 */
void DesktopAwareObject::notifyDesktopModified()
{
    for (auto object : Objects)
        object->desktopModified();
}

/**
 * @short Default implementation.
 *
 * Default implementation moves the window to the closest available desktop.
 */
void DesktopAwareObject::desktopModified()
{
    if (!Widget)
        return;

    if (!Widget->isWindow())
        return;

    QRect rect = QRect(Widget->pos(), Widget->size());
    QRect properRect = properGeometry(rect);
    if (properRect != rect)
    {
        bool visible = Widget->isVisible();
        // workaround for window frame positioning
        // TODO is it still needed?
        Widget->hide();

        // refer to setWindowGeometry() why not using here QWidget::setGeometry()
        Widget->resize(properRect.size());
        Widget->move(properRect.topLeft());

        if (visible)
            Widget->show();
    }
}
