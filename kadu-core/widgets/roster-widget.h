/*
 * %kadu copyright begin%
 * Copyright 2011 Piotr Galiszewski (piotr.galiszewski@kadu.im)
 * Copyright 2011, 2014 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2011, 2013, 2014, 2015 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include "actions/action-context-provider.h"
#include "configuration/configuration-aware-object.h"
#include "exports.h"
#include "os/generic/compositing-aware-object.h"
#include "talkable/talkable.h"

#include <QtCore/QPointer>
#include <QtWidgets/QWidget>
#include <injeqt/injeqt.h>

class Configuration;
class FilteredTreeView;
class GroupTabBar;
class GroupTabBarConfigurator;
class GroupTalkableFilter;
class InjectedFactory;
class ProxyActionContext;
class ModelChain;
class StatusContainerManager;
class TalkableProxyModel;
class TalkableTreeView;

class KADUAPI RosterWidget : public QWidget,
                             public ActionContextProvider,
                             ConfigurationAwareObject,
                             CompositingAwareObject
{
    Q_OBJECT

    QPointer<Configuration> m_configuration;
    QPointer<InjectedFactory> m_injectedFactory;
    QPointer<StatusContainerManager> m_statusContainerManager;

    GroupTabBar *GroupBar;
    QScopedPointer<GroupTabBarConfigurator> TabBarConfigurator;

    TalkableProxyModel *ProxyModel;
    GroupTalkableFilter *MyGroupFilter;

    FilteredTreeView *TalkableWidget;
    TalkableTreeView *TalkableTree;

    ProxyActionContext *Context;

    bool CompositingEnabled;

    void createGui();

    ModelChain *createModelChain();
    void createTalkableWidget(QWidget *parent);

private slots:
    INJEQT_SET void setConfiguration(Configuration *configuration);
    INJEQT_SET void setInjectedFactory(InjectedFactory *injectedFactory);
    INJEQT_SET void setStatusContainerManager(StatusContainerManager *statusContainerManager);
    INJEQT_INIT void init();

protected:
    virtual void configurationUpdated() override;

    virtual void compositingEnabled() override;
    virtual void compositingDisabled() override;

public:
    explicit RosterWidget(QWidget *parent = nullptr);
    virtual ~RosterWidget();

    void clearFilter();

    // TODO 0.11.0: rething
    TalkableTreeView *talkableTreeView();
    TalkableProxyModel *talkableProxyModel();

    // ActionContextProvider implementation
    virtual ActionContext *actionContext() override;

public slots:
    void storeConfiguration();

signals:
    void currentChanged(const Talkable &talkable);
    void talkableActivated(const Talkable &talkable);
};
