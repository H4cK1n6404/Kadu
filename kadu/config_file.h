#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include <qvaluelist.h>
#include <qstring.h>
#include <qrect.h>
#include <qsize.h>
#include <qcolor.h>
#include <qfont.h>
#include <qmap.h>

/**
	Klasa reprezentuj�ca plik z zapisem konfiguracji programu
**/
class ConfigFile {

	private:
		void read();
		void write(const QString &f=QString::null) const;
		bool changeEntry(const QString &group, const QString &name, const QString &value);
		QString getEntry(const QString &group, const QString &name, bool *ok = 0) const;

		QString filename;
		QMap<QString, QMap<QString, QString> > groups;
		mutable QString activeGroupName;
		mutable QMap<QString, QString> *activeGroup;

	public:
		/**
			Otwiera plik filename i go wczytuje
		**/
		ConfigFile(const QString &filename);
		
		/**
			Zapisuje na dysk zawarto�� konfiguracji
		**/
		void sync() const;
		
		/**
			Zapisuje do wybranego pliku kopi� konfiguracji
		**/
		void saveTo(const QString &filename) const;
		
		/**
			Zwraca ca�� sekcj� "name"
		**/
		QMap<QString, QString>& getGroupSection(const QString& name);
		
		/**
			Zwraca list� sekcji
		**/
		QStringList getGroupList() const;
		/**
			Zapisuje warto�� "value" do grupy "group" w pole "name"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param value warto�� QString do wpisania
		**/
		void writeEntry(const QString &group,const QString &name, const QString &value);
		/**
			Zapisuje warto�� "value" do grupy "group" w pole "name"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param value warto�� char* do wpisania
		**/
		void writeEntry(const QString &group,const QString &name, const char *value);
		/**
			Zapisuje warto�� "value" do grupy "group" w pole "name"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param value warto�� int do wpisania
		**/
		void writeEntry(const QString &group,const QString &name, const int value);
		/**
			Zapisuje warto�� "value" do grupy "group" w pole "name"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param value warto�� double do wpisania
		**/
		void writeEntry(const QString &group,const QString &name, const double value);
		/**
			Zapisuje warto�� "value" do grupy "group" w pole "name"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param value warto�� bool do wpisania
		**/
		void writeEntry(const QString &group,const QString &name, const bool value);
		/**
			Zapisuje warto�� "value" do grupy "group" w pole "name"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param value warto�� QRect do wpisania
		**/
		void writeEntry(const QString &group,const QString &name, const QRect &value);
		/**
			Zapisuje warto�� "value" do grupy "group" w pole "name"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param value warto�� QSize do wpisania
		**/
		void writeEntry(const QString &group,const QString &name, const QSize &value);
		/**
			Zapisuje warto�� "value" do grupy "group" w pole "name"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param value warto�� QColor do wpisania
		**/
		void writeEntry(const QString &group,const QString &name, const QColor &value);
		/**
			Zapisuje warto�� "value" do grupy "group" w pole "name"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param value warto�� QFont do wpisania
		**/
		void writeEntry(const QString &group,const QString &name, const QFont &value);
		/**
			Zapisuje warto�� "value" do grupy "group" w pole "name"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param value warto�� QPoint do wpisania
		**/
		void writeEntry(const QString &group,const QString &name, const QPoint &value);
		/**
			Zwraca warto�� pola "name" z grupy "group"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param def domy�lna warto�� pola
			@return warto�� danego pola, je�li nie istnieje zwracana
			jest warto�� "def"
		**/
		QString readEntry(const QString &group,const QString &name, const QString &def = QString::null) const;
		/**
			Zwraca warto�� pola "name" z grupy "group"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param def domy�lna warto�� pola
			@return warto�� danego pola, je�li nie istnieje zwracana
			jest warto�� "def"
		**/
		int readNumEntry(const QString &group,const QString &name, int def = 0) const;
		/**
			Zwraca warto�� pola "name" z grupy "group"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param def domy�lna warto�� pola
			@return warto�� danego pola, je�li nie istnieje zwracana
			jest warto�� "def"
		**/
		unsigned int readUnsignedNumEntry(const QString &group,const QString &name, unsigned int def = 0) const;
		/**
			Zwraca warto�� pola "name" z grupy "group"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param def domy�lna warto�� pola
			@return warto�� danego pola, je�li nie istnieje zwracana
			jest warto�� "def"
		**/
		double readDoubleNumEntry(const QString &group,const QString &name, double def = 0.0) const;
		/**
			Zwraca warto�� pola "name" z grupy "group"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param def domy�lna warto�� pola
			@return warto�� danego pola, je�li nie istnieje zwracana
			jest warto�� "def"
		**/
		bool readBoolEntry(const QString &group,const QString &name, bool def = false) const;
		/**
			Zwraca warto�� pola "name" z grupy "group"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param def domy�lna warto�� pola
			@return warto�� danego pola, je�li nie istnieje zwracana
			jest warto�� "def"
		**/
		QRect readRectEntry(const QString &group,const QString &name, const QRect *def = 0L) const;
		/**
			Zwraca warto�� pola "name" z grupy "group"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param def domy�lna warto�� pola
			@return warto�� danego pola, je�li nie istnieje zwracana
			jest warto�� "def"
		**/
		QSize readSizeEntry(const QString &group,const QString &name, const QSize *def = 0L) const;
		/**
			Zwraca warto�� pola "name" z grupy "group"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param def domy�lna warto�� pola
			@return warto�� danego pola, je�li nie istnieje zwracana
			jest warto�� "def"
		**/
		QColor readColorEntry(const QString &group,const QString &name, const QColor *def = 0L) const;
		/**
			Zwraca warto�� pola "name" z grupy "group"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param def domy�lna warto�� pola
			@return warto�� danego pola, je�li nie istnieje zwracana
			jest warto�� "def"
		**/
		QFont readFontEntry(const QString &group,const QString &name, const QFont *def = 0L) const;
		/**
			Zwraca warto�� pola "name" z grupy "group"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param def domy�lna warto�� pola
			@return warto�� danego pola, je�li nie istnieje zwracana
			jest warto�� "def"
		**/
		QPoint readPointEntry(const QString &group,const QString &name, const QPoint *def = 0L) const;
		/**
			Tworzy pole "name" w grupie "group" je�li ono nie istnieje
			i ustawia warto�� na "value"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param value warto�� QPoint do wpisania
		**/
		void addVariable(const QString &group,const QString &name, const QString &value);
		/**
			Tworzy pole "name" w grupie "group" je�li ono nie istnieje
			i ustawia warto�� na "value"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param value warto�� QPoint do wpisania
		**/
		void addVariable(const QString &group,const QString &name, const char *value);
		/**
			Tworzy pole "name" w grupie "group" je�li ono nie istnieje
			i ustawia warto�� na "value"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param value warto�� QPoint do wpisania
		**/
		void addVariable(const QString &group,const QString &name, const int value);
		/**
			Tworzy pole "name" w grupie "group" je�li ono nie istnieje
			i ustawia warto�� na "value"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param value warto�� QPoint do wpisania
		**/
		void addVariable(const QString &group,const QString &name, const double value);
		/**
			Tworzy pole "name" w grupie "group" je�li ono nie istnieje
			i ustawia warto�� na "value"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param value warto�� QPoint do wpisania
		**/
		void addVariable(const QString &group,const QString &name, const bool value);
		/**
			Tworzy pole "name" w grupie "group" je�li ono nie istnieje
			i ustawia warto�� na "value"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param value warto�� QPoint do wpisania
		**/
		void addVariable(const QString &group,const QString &name, const QRect &value);
		/**
			Tworzy pole "name" w grupie "group" je�li ono nie istnieje
			i ustawia warto�� na "value"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param value warto�� QPoint do wpisania
		**/
		void addVariable(const QString &group,const QString &name, const QSize &value);
		/**
			Tworzy pole "name" w grupie "group" je�li ono nie istnieje
			i ustawia warto�� na "value"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param value warto�� QPoint do wpisania
		**/
		void addVariable(const QString &group,const QString &name, const QColor &value);
		/**
			Tworzy pole "name" w grupie "group" je�li ono nie istnieje
			i ustawia warto�� na "value"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param value warto�� QPoint do wpisania
		**/
		void addVariable(const QString &group,const QString &name, const QFont &value);
		/**
			Tworzy pole "name" w grupie "group" je�li ono nie istnieje
			i ustawia warto�� na "value"
			@param group nazwa grupy
			@param name nazwa pola w grupie
			@param value warto�� QPoint do wpisania
		**/
		void addVariable(const QString &group,const QString &name, const QPoint &value);
};

extern ConfigFile config_file;

#endif
