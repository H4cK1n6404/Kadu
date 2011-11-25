/*
 * %kadu copyright begin%
 * Copyright 2010, 2011 Piotr Galiszewski (piotr.galiszewski@kadu.im)
 * Copyright 2010, 2011 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include <QtGui/QTextDocument>

#include "chat/chat.h"
#include "model/roles.h"

#include "model/dates-model-item.h"
#include "history-tree-item.h"
#include "history.h"

#include "buddy-status-dates-model.h"

BuddyStatusDatesModel::BuddyStatusDatesModel(const Buddy &buddy, const QVector<DatesModelItem> &dates, QObject *parent) :
		QAbstractListModel(parent), MyBuddy(buddy), Dates(dates)
{
}

BuddyStatusDatesModel::~BuddyStatusDatesModel()
{
}

int BuddyStatusDatesModel::columnCount(const QModelIndex &parent) const
{
	return parent.isValid() ? 0 : 1;
}

int BuddyStatusDatesModel::rowCount(const QModelIndex &parent) const
{
	return parent.isValid() ? 0 : Dates.size();
}

QVariant BuddyStatusDatesModel::data(const QModelIndex &index, int role) const
{
	if (!MyBuddy)
		return QVariant();

	int row = index.row();

	if (row < 0 || row >= Dates.size())
		return QVariant();

	switch (role)
	{
		case Qt::DisplayRole: return Dates.at(row).Date.toString("dd.MM.yyyy");
		case HistoryItemRole: return QVariant::fromValue<HistoryTreeItem>(HistoryTreeItem(MyBuddy));
		case BuddyRole: return QVariant::fromValue<Buddy>(MyBuddy);
		case DateRole: return Dates.at(row).Date;
	}

	return QVariant();
}

void BuddyStatusDatesModel::setBuddy(const Buddy &buddy)
{
	MyBuddy = buddy;
}

void BuddyStatusDatesModel::setDates(const QVector<DatesModelItem> &dates)
{
	if (!Dates.isEmpty())
	{
		beginRemoveRows(QModelIndex(), 0, Dates.size() - 1);
		Dates.clear();
		endRemoveRows();
	}

	if (!dates.isEmpty())
	{
		beginInsertRows(QModelIndex(), 0, dates.size() - 1);
		Dates = dates;
		endInsertRows();
	}
}

QModelIndex BuddyStatusDatesModel::indexForDate(const QDate &date)
{
	int i = 0;
	foreach (const DatesModelItem &item, Dates)
	{
		if (item.Date == date)
			return index(i);
		++i;
	}

	return index(-1);
}
