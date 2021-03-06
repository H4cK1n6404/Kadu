/*
 * %kadu copyright begin%
 * Copyright 2014 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2011 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#ifndef LIST_EDIT_WIDGET_H
#define LIST_EDIT_WIDGET_H

#include <QtCore/QModelIndex>
#include <QtWidgets/QWidget>

class QLineEdit;
class QListWidget;

class ListEditWidget : public QWidget
{
    Q_OBJECT

    QListWidget *ListWidget;
    QLineEdit *LineEdit;

    void createGui();

private slots:
    void selectedItemChanged(const QModelIndex &current, const QModelIndex &previous);
    void addItem();
    void changeItem();
    void deleteItem();

public:
    explicit ListEditWidget(QWidget *parent = nullptr);
    virtual ~ListEditWidget();

    void setList(const QStringList &list);
    QStringList list();
};

#endif   // LIST_EDIT_WIDGET_H
