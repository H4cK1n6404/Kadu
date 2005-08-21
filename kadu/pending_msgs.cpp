/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qfile.h>
#include <qtextcodec.h>

#include "chat.h"
#include "chat_manager.h"
#include "config_file.h"
#include "debug.h"
//#include "kadu.h"
#include "pending_msgs.h"
#include "misc.h"

PendingMsgs::PendingMsgs(QObject *parent, const char *name): QObject(parent, name)
{
}

void PendingMsgs::deleteMsg(int index)
{
	kdebugm(KDEBUG_INFO, "PendingMsgs::(pre)deleteMsg(%d), count=%d\n", index, count());
	msgs.remove(msgs.at(index));
	writeToFile();
	kdebugm(KDEBUG_INFO, "PendingMsgs::deleteMsg(%d), count=%d\n", index, count());
	emit messageDeleted();
}

bool PendingMsgs::pendingMsgs(UserListElement user) const
{
//	kdebugf();

//	po co to?
//	if (uin == 0)
//		return pendingMsgs();

	CONST_FOREACH(msg, msgs)
		if((*msg).users[0] == user)
		{
//			kdebugf2();
			return true;
		}
//	kdebugf2();
	return false;
}

bool PendingMsgs::pendingMsgs() const
{
	return !msgs.isEmpty();
}

int PendingMsgs::count() const
{
	return msgs.count();
}

PendingMsgs::Element &PendingMsgs::operator[](int index)
{
	return msgs[index];
}

void PendingMsgs::addMsg(QString protocolName, UserListElements users, QString msg, int msgclass, time_t time)
{
	Element e;
	e.users = users;
	e.proto = protocolName;
	e.msg = msg;
	e.msgclass = msgclass;
	e.time = time;
	msgs.append(e);
	writeToFile();
	emit messageAdded();
}

void PendingMsgs::writeToFile()
{
	QString path = ggPath("kadu.msgs");
	QFile f(path);
	if(!f.open(IO_WriteOnly))
	{
		kdebugmf(KDEBUG_ERROR, "Cannot open file kadu.msgs\n");
		return;
	}
	// Najpierw zapisujemy ilosc wiadomosci
	int t=msgs.count();
	f.writeBlock((char*)&t,sizeof(int));
	// Teraz w petli dla kazdej wiadomosci
	CONST_FOREACH(i, msgs)
	{
		// zapisujemy uiny - najpierw ilosc
		t=(*i).users.size();
		f.writeBlock((char*)&t,sizeof(int));
		// teraz dane
		CONST_FOREACH(j, (*i).users)
		{
			UinType uin = (*j).ID("Gadu").toUInt();
			f.writeBlock((char*)&uin, sizeof(UinType));
		}
		// nastepnie wiadomosc - dlugosc
		t=(*i).msg.length();
		f.writeBlock((char*)&t,sizeof(int));
		// i tresc
		QCString cmsg = codec_latin2->fromUnicode((*i).msg);
		f.writeBlock(cmsg, cmsg.length());
		// na koniec jeszcze klase wiadomosci
		f.writeBlock((char*)&(*i).msgclass,sizeof(int));
		// i czas
		f.writeBlock((char*)&(*i).time,sizeof(time_t));
	}
	// I zamykamy plik
	f.close();
}

bool PendingMsgs::loadFromFile()
{
	QString path = ggPath("kadu.msgs");
	QFile f(path);
	if (!f.open(IO_ReadOnly)) {
		kdebugmf(KDEBUG_WARNING, "Cannot open file kadu.msgs\n");
		return false;
	}

	// Najpierw wczytujemy ilosc wiadomosci
	int msgs_size;
	if (f.readBlock((char*)&msgs_size,sizeof(int)) <= 0) {
		kdebugmf(KDEBUG_ERROR, "kadu.msgs is corrupted\n");
		return false;
	}

	// Teraz w petli dla kazdej wiadomosci
	for (int i = 0; i < msgs_size; ++i)
	{
		Element e;

		// wczytujemy uiny - najpierw ilosc
		int uins_size;
		if (f.readBlock((char*)&uins_size, sizeof(int)) <= 0) {
			--msgs_size;
			return false;
		}

		// teraz dane
		for (int j = 0; j < uins_size; ++j)
		{
			int uin;
			if (f.readBlock((char*)&uin, sizeof(UinType)) <= 0) {
				--msgs_size;
				return false;
			}
			e.users.append(userlist->byID("Gadu", QString::number(uin)));
		}

		// nastepnie wiadomosc - dlugosc
		int msg_size;
		if (f.readBlock((char*)&msg_size, sizeof(int)) <= 0) {
			--msgs_size;
			return false;
		}

		// i tresc
		char *buf = new char[msg_size + 1];
		if (f.readBlock(buf, msg_size) <= 0) {
			--msgs_size;
			delete [] buf;
			return false;
		}
		buf[msg_size] = 0;
		e.msg = codec_latin2->toUnicode(buf);
		delete[] buf;

		// na koniec jeszcze klase wiadomosci
		if (f.readBlock((char*)&e.msgclass, sizeof(int)) <= 0) {
			--msgs_size;
			delete [] buf;
			return false;
		}

		// i czas
		if (f.readBlock((char*)&e.time, sizeof(time_t)) <= 0) {
			--msgs_size;
			delete [] buf;
			return false;
		}

		// dodajemy do listy
		msgs.append(e);
	}

	// I zamykamy plik
	f.close();
	return true;
}

void PendingMsgs::openMessages() {
	chat_manager->openPendingMsgs();
}

PendingMsgs pending(NULL, "pending_msgs");
