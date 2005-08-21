#ifndef WINDOW_NOTIFY_H
#define WINDOW_NOTIFY_H
#include <qobject.h>
#include <qstring.h>

#include "gadu.h"

class WindowNotify : public QObject
{
	Q_OBJECT
	public:
		WindowNotify(QObject *parent=0, const char *name=0);
		~WindowNotify();

	public slots:
		void newChat(const QString &protocolName, UserListElements senders, const QString &msg, time_t t);
		void newMessage(const QString &protocolName, UserListElements senders, const QString &msg, time_t t, bool &grab);
		void connectionError(const QString &protocolName, const QString &message);
		void userChangedStatusToAvailable(const QString &protocolName, UserListElement);
		void userChangedStatusToBusy(const QString &protocolName, UserListElement);
		void userChangedStatusToInvisible(const QString &protocolName, UserListElement);
		void userChangedStatusToNotAvailable(const QString &protocolName, UserListElement);

		void userStatusChanged(UserListElement ule, QString protocolName, const UserStatus &oldStatus);
		void message(const QString &from, const QString &message, const QMap<QString, QVariant> *parameters, const UserListElement *ule);
};

extern WindowNotify *window_notify;

#endif
