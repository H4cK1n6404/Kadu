/*
 * %kadu copyright begin%
 * Copyright 2014 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2013, 2014 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include <QtGui/QIcon>
#include <QtWidgets/QApplication>

#include "icons/icons-manager.h"
#include "icons/kadu-icon.h"

#include "group-filter-tab-data.h"

GroupFilterTabData::GroupFilterTabData(const GroupFilter &filter) : Filter(filter)
{
}

GroupFilter GroupFilterTabData::filter() const
{
    return Filter;
}

QString GroupFilterTabData::tabName() const
{
    switch (Filter.filterType())
    {
    case GroupFilterRegular:
        return Filter.group().showName() ? Filter.group().name() : QString();
    case GroupFilterEverybody:
        return QCoreApplication::translate("GroupFilterTabData", "Everybody");
    case GroupFilterUngroupped:
        return QCoreApplication::translate("GroupFilterTabData", "Ungrouped");
    case GroupFilterInvalid:
    default:
        return QString();
    }
}

QIcon GroupFilterTabData::tabIcon(IconsManager *iconsManager) const
{
    switch (Filter.filterType())
    {
    case GroupFilterRegular:
        return Filter.group().showIcon() ? QIcon(Filter.group().icon()) : QIcon();
    case GroupFilterEverybody:
        return iconsManager->iconByPath(KaduIcon("x-office-address-book"));
    case GroupFilterUngroupped:
    case GroupFilterInvalid:
    default:
        return QIcon();
    }
}
