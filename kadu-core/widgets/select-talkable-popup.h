/*
 * %kadu copyright begin%
 * Copyright 2012 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2012, 2013, 2014 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#pragma once

#include "talkable/talkable.h"

#include "widgets/filtered-tree-view.h"

#include <QtCore/QPointer>
#include <injeqt/injeqt.h>

class HideAnonymousTalkableFilter;
class ModelChain;
class TalkableFilter;
class TalkableProxyModel;
class TalkableTreeView;

class QAbstractItemModel;
class QModelIndex;

class SelectTalkablePopup : public FilteredTreeView
{
    Q_OBJECT

    HideAnonymousTalkableFilter *HideAnonymousFilter;
    ModelChain *Chain;
    TalkableTreeView *View;
    TalkableProxyModel *ProxyModel;

private slots:
    INJEQT_INIT void init();

    void itemClicked(const QModelIndex &index);
    void talkableActivated(const Talkable &talkable);

public:
    explicit SelectTalkablePopup(QWidget *parent = nullptr);
    virtual ~SelectTalkablePopup();

    virtual QSize sizeHint() const;

    void setBaseModel(QAbstractItemModel *model);
    void setShowAnonymous(bool showAnonymous);

    void show(const Talkable &talkable);

    void addFilter(TalkableFilter *filter);
    void removeFilter(TalkableFilter *filter);

signals:
    void talkableSelected(const Talkable &talkable);
};
