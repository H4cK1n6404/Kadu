#ifndef ADDUSER_H
#define ADDUSER_H

#include <qdialog.h>
#include <qlineedit.h>
#include <qlabel.h>
#include "libgadu.h"

/**
	Dialog dodaj�cy nowy kontakt do listy kontakt�w
**/
class Adduser : public QDialog {
	Q_OBJECT

	public:
		Adduser(QDialog* parent=0, const char *name=0);

	protected:
		QLineEdit *e_nickname, *e_uin, *e_fname, *e_lname, *e_altnick, *e_tel;
		QLineEdit *e_group, *e_email;

	protected slots:
		void Add();
};

#endif
