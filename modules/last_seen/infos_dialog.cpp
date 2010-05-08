/*
 * %kadu copyright begin%
 * Copyright 2009 Wojciech Treter (juzefwt@gmail.com)
 * Copyright 2008, 2009, 2010 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * Copyright 2008 Tomasz Rostański (rozteck@interia.pl)
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

#include <QtCore/QStringList>
#include <QtCore/QAbstractItemModel>
#include <QtGui/QTreeWidget>
#include <QtGui/QLayout>
#include <QtGui/QPushButton>
#include <QtNetwork/QHostAddress>

#include "infos_dialog.h"

#include "misc/misc.h"
#include "modules.h"
#include "debug.h"


InfosDialog::InfosDialog(const LastSeen &lastSeen, QWidget *parent, Qt::WindowFlags f)
:QDialog(parent, f)
{
	kdebugf();
	setAttribute(Qt::WA_DeleteOnClose);
//	this->resize(800, this->height());
	this->setWindowTitle(tr("Information about contacts"));

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(5);

	QTreeWidget *listView = new QTreeWidget(this);
	listView->setColumnCount(7);
	listView->setSelectionMode(QAbstractItemView::NoSelection);
	listView->setAllColumnsShowFocus(true);

	QStringList labels;
	labels << tr("Contact") << tr("UIN") << tr("Nick") << tr("IP") << tr("Domain name") << tr("Description") << tr("Last time seen on");
	listView->setHeaderLabels(labels);

	foreach (Contact contact, ContactManager::instance()->items()) {
		if (contact.contactAccount().protocolName().compare("gadu") == 0)
		{
			QString desc, ip;
			if(!contact.currentStatus().description().isEmpty())
				desc = contact.currentStatus().description();
			desc.replace('\n', ' ');
			if(contact.address() != QHostAddress())
				ip = contact.address().toString();

			QStringList labels;
			labels << contact.ownerBuddy().display() << contact.id() << contact.ownerBuddy().nickName() << ip <<
					contact.dnsName() << desc << lastSeen[contact.id()];

			listView->addTopLevelItem(new QTreeWidgetItem(labels));
		}
	}
	listView->sortItems(0, Qt::AscendingOrder);

	QWidget *buttons = new QWidget(this);
	QHBoxLayout *buttonsLayout = new QHBoxLayout(buttons);
	buttonsLayout->setContentsMargins(0, 0, 0, 0);
	buttonsLayout->setSpacing(5);

	QPushButton *closeButton = new QPushButton(tr("&Close"), this);
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(closeButton);

	layout->addWidget(listView);
	layout->addWidget(buttons);

	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
	
	loadWindowGeometry(this, "LastSeen", "LastSeenWidgetGeometry", 0, 0, 800, 300);

	kdebugf2();
}

InfosDialog::~InfosDialog()
{
	kdebugf();

	saveWindowGeometry(this, "LastSeen", "LastSeenWidgetGeometry");

	kdebugf2();
}
