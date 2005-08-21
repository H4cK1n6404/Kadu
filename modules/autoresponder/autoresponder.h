#ifndef AUTORESPONDER_H
#define AUTORESPONDER_H

#include <qobject.h>

#include "config_file.h"
#include "misc.h"
#include "usergroup.h"

class AutoResponder : public QObject
{
	Q_OBJECT
	ConfigFile *config;

	public:
		AutoResponder(QObject *parent=0, const char *name=0);
		~AutoResponder();
	public slots:
		void chatMsgReceived(const QString &protocolName, UserListElements senders, const QString& msg, time_t time, bool& grab);
		void chatOpened(const UserGroup *group);
	public:
		UserGroup replied;		/* tu beda przechowywane uiny tych ktorzy sie odezwali do nas */
};

extern AutoResponder* autoresponder;

#endif
