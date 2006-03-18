#ifndef KADU_USERBOX_H
#define KADU_USERBOX_H

#include <qlistbox.h>
#include <qpixmap.h>
#include <qpopupmenu.h>
#include <qtimer.h>

#include <vector>

#include "gadu.h"
#include "userlistelement.h"

class QFontMetrics;
class UserBoxSlots;
class ULEComparer;

/**
	Klasa reprezentuj�ca kontakt wizualnie na li�cie kontakt�w. Opisuje ona ikon� kontaktu,
	jego wy�wietlan� nazw� oraz ewentualny opis.
	\class KaduListBoxPixmap
	\brief Klasa reprezentuj�ca kontakt wizualnie.
**/
class KaduListBoxPixmap : public QListBoxItem
{
	public:
		/**
			\fn KaduListBoxPixmap(const QPixmap &pix, UserListElement user, bool bold = false)
			Konstruktor dodaj�cy kontakt do listy z podan� ikon�.
			\param pix ikona wy�wietlana obok kontaktu.
			\param user kontakt, z kt�rego b�d� pobierane dane do wy�wietlenia
			\param bold warto�� logiczna informuj�ca o tym, czy nazwa kontaktu
				ma by� wy�wietlana pogrubion� czcionk�.
		**/
		KaduListBoxPixmap(const QPixmap &pix, UserListElement user, bool bold = false);

		/**
			\fn bool isBold() const
			Zwraca informacj�, czy nazwa kontaktu jest wy�wietlana pogrubion� czcionk�, czy nie.
		**/
		bool isBold() const { return bold; }

		/**
			\fn int height(const QListBox *lb) const
			Zwraca wysoko�� elementu reprezentuj�cego wy�wietlany kontakt w pikselach.
			\param lb obiekt reprezentuj�cy wy�wietlany kontakt.
		**/
		int height(const QListBox *lb) const;

		/**
			\fn int width(const QListBox *lb) const
			Zwraca szeroko�� elementu reprezentuj�cego wy�wietlany kontakt w pikselach.
			\param lb obiekt reprezentuj�cy wy�wietlany kontakt.
		**/
		int width(const QListBox *lb) const;

		const UserListElement User;
	protected:
		/**
			\fn int lineHeight(const QListBox *lb) const
			Zwraca wysoko�� samej czcionki dla elementu reprezentuj�cego wy�wietlany kontakt w pikselach.
			\param lb obiekt reprezentuj�cy wy�wietlany kontakt.
		**/
		int lineHeight(const QListBox *lb) const;

		/**
			\fn void paint(QPainter *painter)
			Rysuje wy�wietlany kontakt na li�cie.
			\param painter wska�nik do obiektu rysuj�cego.
		**/
		void paint(QPainter *painter);

		/**
			\fn void setBold(bool b)
			Ustawia stan pogrubienia czcionki wy�wietlanego kontaktu.
			\param b warto�� logiczna okre�laj�ca, czy czcionka ma by� pogrubiona czy nie.
		**/
		void setBold(bool b) { bold = b; }

		/**
			\fn void calculateSize(const QString &text, int width, QStringList &out, int &height) const
			Oblicza odpowiedni� wysoko�� elementu listy kontakt�w tak, aby pomie�ci�
			opis podzielony na niezb�dn� liczb� linii.
			\param[in] text wy�wietlany opis.
			\param[in] width szeroko�� kolumny listy kontakt�w.
			\param[out] out lista �a�cuch�w znak�w, zawieraj�ca kolejne linie opisu powsta�e
				w wyniku podzielenia opisu tak, aby zmie�ci� si� w danej szeroko�ci kolumny.
			\param[out] height wysoko�� elementu listy kontakt�w potrzebna aby pomie�ci� opis
				podzielony w liniach.
		**/
		void calculateSize(const QString &text, int width, QStringList &out, int &height) const;

		/**
			\fn void changeText(const QString &text)
			Zmienia nazw� wy�wietlan� dla kontaktu na li�cie na podan�.
			\param text nowa nazwa do wy�wietlenia.
		**/
		void changeText(const QString &text);
		friend class UserBox;
		friend class UserBoxSlots;

		/*funkcje wprowadzone �eby zaoszcz�dzi� na odwo�aniach do pliku konfiguracyjnego*/
		/**
			\fn static void setFont(const QFont &f)
			Ustawia czcionk� dla tego elementu.
			\param f czcionka
		**/
		static void setFont(const QFont &f);

		/**
			\fn static void setMyUIN(UinType u)
			Ustawia UIN lokalnego u�ytkownika Kadu dla tego elementu, aby by�o wiadomo,
			�e ten element jego reprezentuje (poniewa� do pobierania informacji o statusie i opisie
			u�ywane s� r�ne metody dla kontaktu i lokalnego u�ytkownika).
			\param u numer UIN.
		**/
		static void setMyUIN(UinType u);

		/**
			\fn static void setShowDesc(bool sd)
			Ustawia stan wy�wietlania opis�w na li�cie kontakt�w.
			\param sd warto�� logiczna informuj�ca o tym, czy opisy maj� by� wy�wietlane na li�cie kontat�w.
		**/
		static void setShowDesc(bool sd);

		/**
			\fn static void setAlignTop(bool at)
			Ustawia stan wyr�wnywania do g�rnego brzegu elementu.
			\param at warto�� logiczna informuj�ca o tym, czy wy�wietlana nazwa kontaktu ma by� wyr�wnywana
				do g�rnej kraw�dzi elementu.
		**/
		static void setAlignTop(bool at);

		/**
			\fn static void setShowMultilineDesc(bool m)
			Ustawia stan wy�wietlania opis�w wieloliniowych.
			\param m warto�� logiczna informuj�ca o tym, czy opis tego elementu mo�e by� wy�wietlany
				w wielu liniach, je�li zawiera znaki nowej linii.
		**/
		static void setShowMultilineDesc(bool m);

		/**
			\fn static void setMultiColumn(bool m)
			Ustawia stan wy�wietlania listy kontakt�w w wielu kolumnach.
			\param m warto�� logiczna informuj�ca o tym, czy lista ma by� wy�wietlana w wielu kolumnach.
		**/
		static void setMultiColumn(bool m);

		/**
			\fn static void setMultiColumnWidth(int w)
			Ustawia szeroko�� jednej kolumny dla wy�wietlania listy w trybie wielu kolumn.
			\param w szeroko�� kolumny w pikselach.
		**/
		static void setMultiColumnWidth(int w);

		/**
			\fn static void setDescriptionColor(const QColor &col)
			Ustawia kolor wy�wietlanego opisu.
			\param col kolor
		**/
		static void setDescriptionColor(const QColor &col);

	private:
		QPixmap pm;
		bool bold;
		static QFontMetrics *descriptionFontMetrics;

		static UinType myUIN;
		static bool ShowDesc;
		static bool AlignUserboxIconsTop;
		static bool ShowMultilineDesc;
		static bool MultiColumn;
		static int  MultiColumnWidth;
		static QColor descColor;

		mutable QString buf_text;
		mutable int buf_width;
		mutable QStringList buf_out;
		mutable int buf_height;
};

/**
	Klasa reprezentuj�ca menu podr�czne listy kontakt�w - czyli to, kt�re dostajemy klikaj�c
	na kt�rymkolwiek kontakcie prawym przyciskiem myszy.
	\class UserBoxMenu
	\brief Menu podr�czne listy kontakt�w.
**/
class UserBoxMenu : public QPopupMenu
{
	Q_OBJECT

	private:
		QValueList<QPair<QString, QString> > iconNames;
	private slots:
		void restoreLook();

	public:
		/**
			\fn UserBoxMenu(QWidget *parent=0, const char* name=0)
			Standardowy konstruktor.
			\param parent wska�nik na obiekt kontrolki-rodzica.
			\param name nazwa kontrolki.
		**/
		UserBoxMenu(QWidget *parent=0, const char* name=0);

		/**
			\fn int getItem(const QString &caption) const
			Zwraca unikaln� liczb� identyfikuj�c� element menu z podanym napisem.
			\param caption napis elementu.
		**/
		int getItem(const QString &caption) const;

	public slots:
		/**
			\fn void show(QListBoxItem *item)
			Pokazuje menu dla podanego kontaktu.
			\param item element wy�wietlanej listy kontakt�w reprezentuj�cy rz�dany kontakt.
		**/
		void show(QListBoxItem *item);

		/**
			\fn int addItem(const QString &text, const QObject* receiver, const char* member, const QKeySequence accel= 0, int id= -1)
			Dodaje nowy element do menu.
			\param text napis dla nowego elementu.
			\param receiver obiekt odbieraj�cy sygna� wybrania elementu z menu.
			\param member SLOT obiektu \a receiver kt�ry zostanie wykonany po wybraniu elementu z menu.
			\param accel skr�t klawiaturowy dla tego elementu. Domy�lnie brak.
			\param id Unikatowa liczba identyfikuj�ca nowy element. Domy�lnie pierwsza wolna.
		**/
		int addItem(const QString &text, const QObject* receiver, const char* member, const QKeySequence accel= 0, int id= -1);

		/**
			\fn int addItem(const QString &iconname, const QString &text, const QObject* receiver, const char* member, const QKeySequence accel= 0, int id= -1)
			Dodaje nowy element do menu.
			\param iconname nazwa ikony z zestawu lub sciezka do pliku
			\param text napis dla nowego elementu.
			\param receiver obiekt odbieraj�cy sygna� wybrania elementu z menu.
			\param member SLOT obiektu \a receiver kt�ry zostanie wykonany po wybraniu elementu z menu.
			\param accel skr�t klawiaturowy dla tego elementu. Domy�lnie brak.
			\param id Unikatowa liczba identyfikuj�ca nowy element. Domy�lnie pierwsza wolna.
		**/
		int addItem(const QString &iconname, const QString &text, const QObject* receiver, const char* member, const QKeySequence accel= 0, int id= -1);

		/**
			\fn int addItemAtPos(int index,const QString &iconname, const QString &text, const QObject* receiver, const char* member, const QKeySequence accel= 0, int id= -1)
			Dodaje nowy element do menu.
			\param index pozycja (licz�c od 0) na kt�rej znale�� ma si� nowy element.
			\param iconname nazwa ikony z zestawu lub sciezka do pliku
			\param text napis dla nowego elementu.
			\param receiver obiekt odbieraj�cy sygna� wybrania elementu z menu.
			\param member SLOT obiektu \a receiver kt�ry zostanie wykonany po wybraniu elementu z menu.
			\param accel skr�t klawiaturowy dla tego elementu. Domy�lnie brak.
			\param id Unikatowa liczba identyfikuj�ca nowy element. Domy�lnie pierwsza wolna.
		**/
		int addItemAtPos(int index,const QString &iconname, const QString &text, const QObject* receiver, const char* member, const QKeySequence accel= 0, int id= -1);

		/**
			\fn void refreshIcons()
			Prze�adowuje wszystkie ikony w tym menu dodane przy pomocy powy�szych funkcji
		**/
		void refreshIcons();
	signals:
		/**
			\fn void popup()
			Sygna� emitowany, gdy menu jest wywo�ywane.
		**/
		void popup();
};


/**
	Klasa reprezentuj�ca list� kontakt�w wraz z ikonkami stan�w.
	\class UserBox
	\brief Wy�wietlana lista kontakt�w.
**/
class UserBox : public QListBox
{
	Q_OBJECT

	public:
		/**
			\fn UserBox(UserGroup *group = userlist, QWidget* parent = 0, const char* name = 0, WFlags f = 0)
			Standardowy konstruktor tworz�cy list� kontakt�w.
			\param group grupa kontakt�w, kt�ra b�dzie wy�wietlana
			\param parent rodzic kontrolki. Domy�lnie 0.
			\param name nazwa kontrolki. Domy�lnie 0.
			\param f flagi kontrolki. Domy�lnie 0.
		**/
		UserBox(UserGroup *group = userlist, QWidget* parent = 0, const char* name = 0, WFlags f = 0);

		~UserBox();

		/**
			\var static UserBoxMenu *userboxmenu
			Wska�nik do menu kontekstowego listy kontakt�w.
		**/
		static UserBoxMenu *userboxmenu;

		static void setColorsOrBackgrounds();

		/**
			\fn static void initModule()
			Inicjalizuje zmienne niezb�dne do dzia�ania UserBox. Funkcja ta jest
			wywo�ywana przy starcie Kadu przez rdze� Kadu.
		**/
		static void initModule();

		static void closeModule();

		/**
			\fn UserListElements getSelectedUsers() const
			Funkcja zwraca list� zaznaczonych uzytkownik�w.
			\return UserListElements z zaznaczonymi u�ytkownikami.
		**/
		UserListElements selectedUsers() const;

		/**
			\fn const UserGroup *visibleUsers() const
			zwraca list� kontakt�w, kt�re s� obecnie na li�cie
			\return grupa kontakt�w
		**/
		const UserGroup *visibleUsers() const;

		/**
			\fn QValueList<UserGroup *> filters() const
			Zwraca list� filtr�w "pozytywnych"
			\return lista filtr�w
		**/
		QValueList<UserGroup *> filters() const;

		/**
			\fn QValueList<UserGroup *> negativeFilters() const
			Zwraca list� filtr�w "negatywnych"
			\return lista filtr�w
		**/
		QValueList<UserGroup *> negativeFilters() const;

		/**
			\fn bool currentUserExists() const
			\return informacja o istnieniu bie��cego kontaktu
		**/
		bool currentUserExists() const;

		/**
			\fn UserListElement currentUser() const
			\attention {przed wywo�aniem tej metody nale�y sprawdzi�, czy istnieje bie��cy kontakt!}
			\return bie��cy kontakt
			\see UserBox::currentUserExists()
		**/
		UserListElement currentUser() const;

		/**
			\fn static UserBox* activeUserBox()
			Funkcja znajdujaca aktywny UserBox.
			\return wska�nik do aktywnego UserBox'a, je�li taki nie istnieje zwracana jest warto�� NULL.
		**/
		static UserBox* activeUserBox();

		class CmpFuncDesc
		{
			public:
 				CmpFuncDesc(QString i, QString d, int (*f)(const UserListElement &, const UserListElement &))
 					: id(i), description(d), func(f) {}
 				CmpFuncDesc(){}

				QString id;
				QString description;
				int (*func)(const UserListElement &, const UserListElement &);
		};

		/**
			\fn QValueList<UserBox::CmpFuncDesc> compareFunctions() const
			\return lista obiekt�w opisuj�cych funkcje por�wnuj�ce
		**/
 		QValueList<UserBox::CmpFuncDesc> compareFunctions() const;

		/**
			\fn void addCompareFunction(const QString &id, const QString &trDescription, int (*cmp)(const UserListElement &, const UserListElement &))
			Dodaje funkcj� por�wnuj�c� do bie��cego UserBoksa.
			Funkcja cmp powinna zwraca� warto��:
				< 0 gdy pierwszy kontakt powinien znale�� si� przed drugim
				= 0 gdy kolejno�� kontakt�w nie ma znaczenia
				> 0 gdy pierwszy kontakt powinien znale�� si� za drugim
			\param id kr�tki identyfikator funkcji
			\param trDescription pzet�umaczony opis funkcji (w zamierzeniu do wy�wietlenia w konfiguracji)
			\param cmp funkcja por�wnuj�ca
		**/
		void addCompareFunction(const QString &id, const QString &trDescription,
					int (*cmp)(const UserListElement &, const UserListElement &));

		/**
			\fn static void refreshAll()
			Od�wie�a wszystkie UserBoksy.
		**/
		static void refreshAll();

		/**
			\fn static void refreshAll()
			Od�wie�a wszystkie UserBoksy, ale dopiero po powrocie do p�tli zdarze� Qt.
		**/
		static void refreshAllLater();
	public slots:

		/**
			\fn void applyFilter(UserGroup *group)
			Nak�ada na bie��cy UserBox filtr "pozytywny" - wy�wietlane b�d�
			tylko te kontakty, kt�re nale�� do group.
			\param group filtr
		**/
		void applyFilter(UserGroup *group);

		/**
			\fn void removeFilter(UserGroup *group)
			Usuwa z bie��cego UserBoksa wskazany filtr "pozytywny".
			\param group filtr
		**/
		void removeFilter(UserGroup *group);

		/**
			\fn void applyNegativeFilter(UserGroup *group)
			Nak�ada na bie��cy UserBox filtr "negatywny" - wy�wietlane b�d�
			tylko te kontakty, kt�re nie nale�� do group.
			\param group filtr
		**/
		void applyNegativeFilter(UserGroup *group);

		/**
			\fn void removeNegativeFilter(UserGroup *group)
			Usuwa z bie��cego UserBoksa wskazany filtr "negatywny".
			\param group filtr
		**/
		void removeNegativeFilter(UserGroup *group);

		/**
			\fn void removeCompareFunction(const QString &id)
			Usuwa funkcj� por�wnuj�c� o identyfikatorze id.
			\param id identyfikator funkcji por�wnuj�cej
		**/
		void removeCompareFunction(const QString &id);

		/**
			\fn void moveUpCompareFunction(const QString &id)
			Przesuwa funkcj� por�wnuj�c� o identyfikatorze id wy�ej w kolejno�ci sprawdzania.
			\param id identyfikator funkcji por�wnuj�cej
		**/
		void moveUpCompareFunction(const QString &id);

		/**
			\fn void moveDownCompareFunction(const QString &id)
			Przesuwa funkcj� por�wnuj�c� o identyfikatorze id ni�ej w kolejno�ci sprawdzania.
			\param id identyfikator funkcji por�wnuj�cej
		**/
 		void moveDownCompareFunction(const QString &id);

		/**
			\fn void refresh()
			Od�wie�a bie��cy UserBox. Nie nale�y tej funkcji nadu�ywa�,
			bo wi�kszo�� (90%) sytuacji jest wykrywanych przez sam� klas�.
		**/
		void refresh();

		void refreshLater();
	signals:
		/**
			\fn void doubleClicked(UserListElement user)
			Sygna� jest emitowany, gdy na kt�rym� z kontakt�w klikni�to dwukrotnie.
			\param user kontakt, na kt�rym kilkni�to dwukrotnie
			\warning U�ywaj tego sygna�u zamiast QListBox::doubleClicked(QListBoxItem *) !!!
			Tamten ze wzgl�du na od�wie�anie listy w jednym ze slot�w pod��czonych
			do tego sygna�u czasami przekazuje wska�nik do elementu, kt�ry ju� NIE ISTNIEJE.
		**/
		void doubleClicked(UserListElement user);

		/**
			\fn void returnPressed(UserListElement user)
			Sygna� jest emitowany, gdy wci�ni�to klawisz enter na wybranym kontakcie.
			\param user kontakt, na kt�rym wci�ni�to enter
			\warning U�ywaj tego sygna�u zamiast QListBox::returnPressed(QListBoxItem *) !!!
			Tamten ze wzgl�du na od�wie�anie listy w jednym ze slot�w pod��czonych
			do tego sygna�u czasami przekazuje wska�nik do elementu, kt�ry ju� NIE ISTNIEJE.
		**/
		void returnPressed(UserListElement user);

		/**
			\fn void currentChanged(UserListElement user)
			Sygna� jest emitowany, gdy zmieni� si� bie��cy kontakt.
			\attention {raczej nale�u u�ywa� tego sygna�u zamiast QListBox::currentChaned(QListBoxItem *)}
			\param user obecnie bie��cy kontakt
		**/
		void currentChanged(UserListElement user);

		void changeToolTip(const QPoint &point, UserListElement user, bool show);

	private slots:
		void doubleClickedSlot(QListBoxItem *item);
		void returnPressedSlot(QListBoxItem *item);
		void currentChangedSlot(QListBoxItem *item);

		void userAddedToVisible(UserListElement elem, bool massively, bool last);
		void userRemovedFromVisible(UserListElement elem, bool massively, bool last);

		void userAddedToGroup(UserListElement elem, bool massively, bool last);
		void userRemovedFromGroup(UserListElement elem, bool massively, bool last);

		void userDataChanged(UserListElement elem, QString name, QVariant oldValue,
							QVariant currentValue, bool massively, bool last);
		void protocolUserDataChanged(QString protocolName, UserListElement elem,
							QString name, QVariant oldValue, QVariant currentValue,
							bool massively, bool last);
		void removingProtocol(UserListElement elem, QString protocolName, bool massively, bool last);

		void tipTimeout();
		void restartTip(const QPoint &p);
		void hideTip();
		void resetVerticalPosition();
		void rememberVerticalPosition();

	private:
		static QValueList<UserBox*> UserBoxes;
		static UserBoxSlots *userboxslots;

		UserGroup *VisibleUsers;
		QValueList<UserGroup *> Filters;
		QValueList<UserGroup *> NegativeFilters;
		std::vector<UserListElement> sortHelper;
		std::vector<UserListElement> toRemove;
		QMap<const UserGroup *, UserListElements> AppendProxy;
		QMap<const UserGroup *, UserListElements> RemoveProxy;

		ULEComparer *comparer;
		void sort();
		QTimer refreshTimer;

		QString lastMouseStopUser;
		QPoint lastMouseStop;
		bool tipAlive;
		QTimer tipTimer;

		QTimer verticalPositionTimer;
		int lastVerticalPosition;

		friend class UserBoxSlots;

	protected:
		virtual void wheelEvent(QWheelEvent *e);
		virtual void enterEvent(QEvent *);
		virtual void leaveEvent(QEvent *);

		/**
			\fn virtual void mousePressEvent(QMouseEvent *e)
			Wci�ni�to kt�ry� z przycisk�w myszki na li�cie kontakt�w.
			\param e wska�nik obiektu opisuj�cego to zdarzenie.
		**/
		virtual void mousePressEvent(QMouseEvent *e);

		/**
			\fn virtual void mouseMoveEvent(QMouseEvent* e)
			Poruszono myszk� nad list� kontakt�w.
			\param e wska�nik obiektu opisuj�cego to zdarzenie.
		**/
		virtual void mouseMoveEvent(QMouseEvent* e);

		/**
			\fn virtual void keyPressEvent(QKeyEvent *e)
			Wci�ni�to kt�ry� z klawisz�w w aktywnej li�cie kontakt�w.
			\param e wska�nik obiektu opisuj�cego to zdarzenie.
		**/
		virtual void keyPressEvent(QKeyEvent *e);

		/**
			\fn virtual void resizeEvent(QResizeEvent *)
			Lista kontakt�w zmieni�a sw�j rozmiar.
			\param e wska�nik obiektu opisuj�cego to zdarzenie.
		**/
		virtual void resizeEvent(QResizeEvent *);
};

/**
	Klasa ta pozwala na podgl�d wprowadzanych zmian konfiguracyjnych wobec UserBox
	w oknie konfiguracji, zanim zaaplikuje si� je do Kadu.
	\class UserBoxSlots
	\brief Obs�uga UserBox w konfiguracji.
**/
class UserBoxSlots : public QObject
{
	Q_OBJECT
	public slots:
		/**
			\fn void onCreateTabLook()
			Obs�uguje sekcj� UserBox podczas otwierania okna konfiguracji.
		**/
		void onCreateTabLook();

		/**
			\fn void onDestroyConfigDialog()
			Obs�uguje sekcj� UserBox podczas zamykania okna konfiguracji.
		**/
		void onApplyTabLook();

		/**
			\fn void chooseColor(const char *name, const QColor& color)
			Od�wie�a podgl�d wybranego koloru.
			\param name nazwa elementu, dla kt�rego wybrano kolor.
				\arg \c userbox_bg_color oznacza kolor t�a.
				\arg \c userbox_font_color oznacz kolor czcionki.
			\param color wybrany kolor.
		**/
		void chooseColor(const char *name, const QColor& color);

		/**
			\fn void chooseFont(const char *name, const QFont& font)
			Od�wie�a podgl�d wybranej czcionki.
			\param name nazwa elementu, dla kt�rego wybrano czcionk�.
				\arg \c userbox_font_box oznacza og�ln� czcionk� listy kontakt�w.
			\param font wybrana czcionka.
		**/
		void chooseFont(const char *name, const QFont& font);

		/**
			\fn void onMultiColumnUserbox(bool toggled)
			W��cza b�d� wy��cza kontrolk� szeroko�ci kolumny listy kontakt�w, w zale�no�ci od podanego argumentu.
			\param togglet warto�� logiczna informuj�ca, czy kontrolka ma by� w��czona czy wy��czona.
		**/
		void onMultiColumnUserbox(bool toggled);

		/**
			\fn void updatePreview()
			Od�wie�a podgl�d wszystkich element�w UserBox'a.
		**/
		void updatePreview();

		void chooseBackgroundFile();
		void userboxBackgroundMove(bool toggled);
	private slots:
		void backgroundFileChanged(const QString &text);
};

/**
	\fn int compareAltNick(const UserListElement &u1, const UserListElement &u2)
	Funkcja por�wnuj�ca AltNicki metod� QString::localeAwareCompare()
**/
int compareAltNick(const UserListElement &u1, const UserListElement &u2);

/**
	\fn int compareStatus(const UserListElement &u1, const UserListElement &u2)
	Funkcja por�wnuj�ca statusy w protokole Gadu-Gadu. Uwa�a status "dost�pny" i "zaj�ty" za r�wnowa�ne.
**/
int compareStatus(const UserListElement &u1, const UserListElement &u2);

#endif
