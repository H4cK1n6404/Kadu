#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <qtoolbar.h>
#include "dockarea.h"
#include "toolbutton.h"
#include "usergroup.h"

class ToolButton;

/**
	Klasa tworząca pasek narzędziowy
	\class ToolBar
	\brief Pasek narzędziowy
**/

class ToolBar : public QToolBar
{
	Q_OBJECT

	private:
		ToolButton *dragButton; /*!< przeciągany przycisk akcji */

	private slots:
		/**
			\fn void addButtonClicked()
			Slot dodający wybrany przycisk
		**/
		void addButtonClicked(int action_index);

		/**
			\fn void deleteToolbar()
			Slot obsługujący usuwanie paska narzędzi
		**/
		void deleteToolbar(void);

	protected:
		/**
			\fn virtual void dragEnterEvent(QDragEnterEvent* event)
			Funkcja obsługująca przęciąganie akcji między paskami
		**/
		virtual void dragEnterEvent(QDragEnterEvent* event);

		/**
			\fn virtual void dropEvent(QDropEvent* event)
			Funkcja obsługująca upuszczenie przycisku na pasku
		**/
		virtual void dropEvent(QDropEvent* event);

		/**
			\fn virtual void contextMenuEvent(QContextMenuEvent* e)
			Funkcja obsługująca tworzenie menu kontekstowego paska
		**/
		virtual void contextMenuEvent(QContextMenuEvent* e);

		/**
			\fn virtual void dragLeaveEvent(QDragLeaveEvent *e)
			Funkcja obsługująca upuszczanie przycisku
		**/
		virtual void dragLeaveEvent(QDragLeaveEvent *e);

		/**
			\fn virtual void contextMenuEvent(QContextMenuEvent* e)
			Funkcja obsługująca przeciąganie paska narzędziowego.
		**/
		virtual void moveEvent(QMoveEvent *e);

	public:
		/**
			Konstruktor paska narzędzi
			\fn ToolBar(QWidget* parent, const char *name)
			\param parent rodzic obiektu
			\param name nazwa obiektu
		**/
		ToolBar(QWidget* parent, const char *name);

		/**
			\fn ~ToolBar()
			Destruktor paska narzędzi
		**/
		~ToolBar();

		/**
			\fn DockArea* dockArea()
			Zwraca wskaźnik do rodzica paska (miejsca dokowania)
		**/
		DockArea* dockArea();

		/**
			\fn void loadFromConfig(QDomElement parent_element)
			\param parent_element rodzic obiektu
			Wczytuje dane z pliku konfiguracyjnego
		**/
		void loadFromConfig(QDomElement parent_element);

		/**
			\fn const UserGroup* selectedUsers() const
			Returns list of users that will be affected by activated action.
			It depends on where the toolbar is located. If toolbar is in chat
			window, selected users are the users in chat. If toolbar is the
			main toolbar, selected users are the selected ones in contact
			list, and so on...
			Returns NULL if toolbar is not connected to user list.
		**/
		const UserGroup* selectedUsers() const;

		/**
			\fn QPopupMenu* createContextMenu(QWidget* parent)
			\param parent rodzic obiektu
			Funkcja tworząca menu kontekstowe, umożliwiające dodanie
			nowych akcji do paska narzędziowego.
		**/
		QPopupMenu* createContextMenu(QWidget* parent);

		/**
			\fn hasAction(QString action_name)
			\param action_name nazwa szukanej akcji
			Funkcja zwraca wartość boolowską, określającą, czy akcja
			o podanej nazwie znajduje się już na pasku narzędzi.
		**/
		bool hasAction(QString action_name);

	public slots:
		/**
			\fn writeToConfig(QDomElement parent_element)
			\param parent_element rodzic obiektu
			Zapisuje ustawienia paska (jak offset), oraz (pośrednio) 
			akcje znajdujące się na pasku.
		**/
		void writeToConfig(QDomElement parent_element);
};

#endif
