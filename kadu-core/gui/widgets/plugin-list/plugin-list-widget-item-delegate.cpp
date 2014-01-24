/*
 * %kadu copyright begin%
 * Copyright 2012 Marcel Zięba (marseel@gmail.com)
 * Copyright 2012 Wojciech Treter (juzefwt@gmail.com)
 * Copyright 2012, 2013 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * Copyright 2012, 2013 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * %kadu copyright end%
 *
 * This file is derived from part of the KDE project
 * Copyright (C) 2007, 2006 Rafael Fernández López <ereslibre@kde.org>
 * Copyright (C) 2002-2003 Matthias Kretz <kretz@kde.org>
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

#define MARGIN 5

#include <QtGui/QApplication>
#include <QtGui/QBoxLayout>
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPainter>
#include <QtGui/QPushButton>
#include <QtGui/QStyleOptionViewItemV4>

#include "configuration/configuration-manager.h"
#include "core/core.h"
#include "gui/widgets/categorized-list-view-painter.h"
#include "gui/widgets/categorized-list-view.h"
#include "gui/widgets/plugin-list/plugin-list-view-delegate.h"
#include "gui/widgets/plugin-list/plugin-list-widget.h"
#include "gui/windows/message-dialog.h"
#include "icons/kadu-icon.h"
#include "model/categorized-sort-filter-proxy-model.h"
#include "plugin/metadata/plugin-metadata.h"
#include "plugin/model/plugin-model.h"
#include "plugin/model/plugin-model.h"
#include "plugin/model/plugin-proxy-model.h"
#include "plugin/plugin-dependency-handler.h"

#include "plugin-list-widget-item-delegate.h"

PluginListWidgetItemDelegate::PluginListWidgetItemDelegate(PluginListWidget *pluginSelector_d, QObject *parent)
                : PluginListWidgetDelegate(pluginSelector_d->m_listView, parent)
                , checkBox(new QCheckBox)
                , pushButton(new QPushButton)
                , pluginSelector_d(pluginSelector_d)
{
        pushButton->setIcon(KaduIcon("preferences-other").icon()); // only for getting size matters
}

PluginListWidgetItemDelegate::~PluginListWidgetItemDelegate()
{
        delete checkBox;
        delete pushButton;
}

void PluginListWidgetItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
        if (!index.isValid())
        {
                return;
        }

        int xOffset = checkBox->sizeHint().width();
#ifdef Q_OS_WIN32
        xOffset += QApplication::style()->pixelMetric(QStyle::PM_CheckBoxLabelSpacing);
#endif

        painter->save();

        QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, 0);

        int iconSize = -MARGIN;

        QRect contentsRect(pluginSelector_d->dependantLayoutValue(MARGIN * 2 + iconSize + option.rect.left() + xOffset, option.rect.width() - MARGIN * 3 - iconSize - xOffset, option.rect.width()), MARGIN + option.rect.top(), option.rect.width() - MARGIN * 3 - iconSize - xOffset, option.rect.height() - MARGIN * 2);

        int lessHorizontalSpace = MARGIN * 2 + pushButton->sizeHint().width();

        contentsRect.setWidth(contentsRect.width() - lessHorizontalSpace);

        if (option.state & QStyle::State_Selected)
        {
                painter->setPen(option.palette.highlightedText().color());
        }

        if (pluginSelector_d->m_listView->layoutDirection() == Qt::RightToLeft)
        {
                contentsRect.translate(lessHorizontalSpace, 0);
        }

        painter->save();

        QFont font = titleFont(option.font);
        QFontMetrics fmTitle(font);
        painter->setFont(font);
        painter->drawText(contentsRect, Qt::AlignLeft | Qt::AlignTop, fmTitle.elidedText(index.model()->data(index, Qt::DisplayRole).toString(), Qt::ElideRight, contentsRect.width()));
        painter->restore();
        painter->drawText(contentsRect, Qt::AlignLeft | Qt::AlignBottom, option.fontMetrics.elidedText(index.model()->data(index, PluginModel::CommentRole).toString(), Qt::ElideRight, contentsRect.width()));

        QFont subfont = subtitleFont(option.font);
        QFontMetrics fmSubtitle(subfont);
        painter->setFont(subfont);
        painter->drawText(contentsRect, Qt::AlignLeft | Qt::AlignVCenter, fmSubtitle.elidedText(index.model()->data(index, PluginModel::NameRole).toString(), Qt::ElideRight, contentsRect.width()));

        painter->restore();
}

QSize PluginListWidgetItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
        int i = 4;
        int j = 1;

        QFont font = titleFont(option.font);

        QFontMetrics fmTitle(font);

        return QSize(qMax(fmTitle.width(index.model()->data(index, Qt::DisplayRole).toString()),
                          option.fontMetrics.width(index.model()->data(index, PluginModel::CommentRole).toString())) +
			  + MARGIN * i + pushButton->sizeHint().width() * j,
                     qMax(MARGIN * 2, fmTitle.height() * 2 + option.fontMetrics.height() + MARGIN * 2));
}

QList<QWidget*> PluginListWidgetItemDelegate::createItemWidgets() const
{
        QList<QWidget*> widgetList;

        QCheckBox *enabledCheckBox = new QCheckBox;
        connect(enabledCheckBox, SIGNAL(clicked(bool)), this, SLOT(slotStateChanged(bool)));
        connect(enabledCheckBox, SIGNAL(clicked(bool)), this, SLOT(emitChanged()));

        QPushButton *aboutPushButton = new QPushButton;
        aboutPushButton->setIcon(KaduIcon("help-contents").icon());
        connect(aboutPushButton, SIGNAL(clicked(bool)), this, SLOT(slotAboutClicked()));

        QPushButton *configurePushButton = new QPushButton;
        configurePushButton->setIcon(KaduIcon("preferences-other").icon());
        connect(configurePushButton, SIGNAL(clicked(bool)), this, SLOT(slotConfigureClicked()));

        setBlockedEventTypes(enabledCheckBox, QList<QEvent::Type>() << QEvent::MouseButtonPress
                             << QEvent::MouseButtonRelease << QEvent::MouseButtonDblClick
                             << QEvent::KeyPress << QEvent::KeyRelease);

        setBlockedEventTypes(aboutPushButton, QList<QEvent::Type>() << QEvent::MouseButtonPress
                             << QEvent::MouseButtonRelease << QEvent::MouseButtonDblClick
                             << QEvent::KeyPress << QEvent::KeyRelease);

        setBlockedEventTypes(configurePushButton, QList<QEvent::Type>() << QEvent::MouseButtonPress
                             << QEvent::MouseButtonRelease << QEvent::MouseButtonDblClick
                             << QEvent::KeyPress << QEvent::KeyRelease);

        widgetList << enabledCheckBox << configurePushButton << aboutPushButton;

        return widgetList;
}

void PluginListWidgetItemDelegate::updateItemWidgets(const QList<QWidget*> widgets,
                const QStyleOptionViewItem &option,
                const QPersistentModelIndex &index) const
{
        QCheckBox *checkBox = static_cast<QCheckBox*>(widgets[0]);
        checkBox->resize(checkBox->sizeHint());
        checkBox->move(pluginSelector_d->dependantLayoutValue(MARGIN, checkBox->sizeHint().width(), option.rect.width()), option.rect.height() / 2 - checkBox->sizeHint().height() / 2);

        QPushButton *aboutPushButton = static_cast<QPushButton*>(widgets[2]);
        QSize aboutPushButtonSizeHint = aboutPushButton->sizeHint();
        aboutPushButton->resize(aboutPushButtonSizeHint);
        aboutPushButton->move(pluginSelector_d->dependantLayoutValue(option.rect.width() - MARGIN - aboutPushButtonSizeHint.width(), aboutPushButtonSizeHint.width(), option.rect.width()), option.rect.height() / 2 - aboutPushButtonSizeHint.height() / 2);

        QPushButton *configurePushButton = static_cast<QPushButton*>(widgets[1]);
        QSize configurePushButtonSizeHint = configurePushButton->sizeHint();
        configurePushButton->resize(configurePushButtonSizeHint);
        configurePushButton->move(pluginSelector_d->dependantLayoutValue(option.rect.width() - MARGIN * 2 - configurePushButtonSizeHint.width() - aboutPushButtonSizeHint.width(), configurePushButtonSizeHint.width(), option.rect.width()), option.rect.height() / 2 - configurePushButtonSizeHint.height() / 2);

        if (!index.isValid() || !index.internalPointer())
        {
                checkBox->setVisible(false);
                aboutPushButton->setVisible(false);
                configurePushButton->setVisible(false);
        }
        else
        {
                checkBox->setChecked(index.model()->data(index, Qt::CheckStateRole).toBool());
				configurePushButton->setVisible(false);
        }
}

void PluginListWidgetItemDelegate::slotStateChanged(bool state)
{
        if (!focusedIndex().isValid())
                return;

        const_cast<QAbstractItemModel*>(focusedIndex().model())->setData(focusedIndex(), state, Qt::CheckStateRole);
}

void PluginListWidgetItemDelegate::emitChanged()
{
        emit changed(true);
}

void PluginListWidgetItemDelegate::slotAboutClicked()
{
	const QModelIndex index = focusedIndex();
	const QAbstractItemModel *model = index.model();

	QString info;
	info += tr("Plugin name: %1").arg(model->data(index, PluginModel::NameRole).toString()) + "\n";

	auto pluginMetadata = model->data(index, PluginModel::MetadataRole).value<PluginMetadata>();
	info += tr("Author: %1").arg(pluginMetadata.author()) + "\n";
	info += tr("Version: %1").arg(pluginMetadata.version()) + "\n";
	info += tr("Description: %1").arg(pluginMetadata.description()) + "\n";
	info += tr("Dependencies: %1").arg(pluginMetadata.dependencies().join(", ")) + "\n";
	info += tr("Provides: %1").arg(pluginMetadata.provides());

	MessageDialog::show(KaduIcon("dialog-information"), tr("Plugin information"), info, QMessageBox::Ok, itemView());
}

void PluginListWidgetItemDelegate::slotConfigureClicked()
{
}

QFont PluginListWidgetItemDelegate::titleFont(const QFont &baseFont) const
{
        QFont retFont(baseFont);
        retFont.setBold(true);

        return retFont;
}

QFont PluginListWidgetItemDelegate::subtitleFont(const QFont &baseFont) const
{
        QFont retFont(baseFont);
        retFont.setItalic(true);
	retFont.setPointSize(baseFont.pointSize()-2);

        return retFont;
}

#include "moc_plugin-list-widget-item-delegate.cpp"
