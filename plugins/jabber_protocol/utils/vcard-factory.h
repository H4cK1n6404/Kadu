/*
 * %kadu copyright begin%
 * Copyright 2010, 2011 Piotr Galiszewski (piotr.galiszewski@kadu.im)
 * Copyright 2009, 2009, 2010 Wojciech Treter (juzefwt@gmail.com)
 * Copyright 2010, 2011 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * Copyright 2010 Bartosz Brachaczek (b.brachaczek@gmail.com)
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

/*
 * vcardfactory.h - class for caching vCards
 * Copyright (C) 2003  Michail Pishchagin
 */

#ifndef VCARD_FACTORY_H
#define VCARD_FACTORY_H

#include <QMap>
#include <QObject>
#include <QStringList>

#include "xmpp_tasks.h"
#include "xmpp_vcard.h"

namespace XMPP {
	class VCard;
	class Jid;
	class Task;
	class JT_VCard;
}

class VCardFactory : public QObject
{
	Q_OBJECT

public:
	static void createInstance();
	static void destroyInstance();
	static VCardFactory * instance();
	void setVCard(const XMPP::Jid &, const XMPP::VCard &);
	void setVCard(XMPP::Task *rootTask, const XMPP::Jid &, const XMPP::VCard &v, QObject *obj = 0, const char *slot = 0);

protected:
	void checkLimit(QString jid, XMPP::VCard *vcard);

private slots:
	void updateVCardFinished();

private:
	explicit VCardFactory(QObject *parent = 0);
	virtual ~VCardFactory();

	static VCardFactory* instance_;
	const int dictSize_;
	QStringList vcardList_;
	QMap<QString, XMPP::VCard *> vcardDict_;

	void saveVCard(const XMPP::Jid &, const XMPP::VCard &);
};

#endif
