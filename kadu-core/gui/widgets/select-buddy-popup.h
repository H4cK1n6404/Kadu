/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SELECT_BUDDY_POPUP_H
#define SELECT_BUDDY_POPUP_H

#include "gui/widgets/buddies-list-widget.h"

class QModelIndex;

class Buddy;

class SelectBuddyPopup : public BuddiesListWidget
{
	Q_OBJECT

private slots:
	void itemClicked(const QModelIndex &index);

public:
	explicit SelectBuddyPopup(QWidget *parent = 0);
	virtual ~SelectBuddyPopup();

	void show(Buddy buddy);

signals:
	void buddySelected(Buddy buddy);

};

#include "buddies/buddy.h" // for MOC

#endif // SELECT_BUDDY_POPUP_H
