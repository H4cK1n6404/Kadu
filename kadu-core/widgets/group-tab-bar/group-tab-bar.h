/*
 * %kadu copyright begin%
 * Copyright 2014 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2013, 2014, 2015 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#ifndef GROUP_TAB_BAR_H
#define GROUP_TAB_BAR_H

#include <QtCore/QPointer>
#include <QtWidgets/QTabBar>
#include <injeqt/injeqt.h>

#include "buddies/buddy-list.h"
#include "buddies/group.h"
#include "widgets/group-tab-bar/group-tab-bar-configuration.h"

class BuddyListMimeDataService;
class ChatListMimeDataService;
class Chat;
class GroupManager;
class GroupFilter;
class IconsManager;
class InjectedFactory;
class KaduWindowService;

class KADUAPI GroupTabBar : public QTabBar
{
    Q_OBJECT

    QPointer<BuddyListMimeDataService> m_buddyListMimeDataService;
    QPointer<ChatListMimeDataService> m_chatListMimeDataService;
    QPointer<GroupManager> m_groupManager;
    QPointer<IconsManager> m_iconsManager;
    QPointer<InjectedFactory> m_injectedFactory;
    QPointer<KaduWindowService> m_kaduWindowService;

    GroupTabBarConfiguration m_groupTabBarConfiguration;
    GroupFilter m_groupFilter;

    // for dnd support
    BuddyList DNDBuddies;
    QList<Chat> DNDChats;

    int indexOf(GroupFilter groupFilter);
    void insertGroupFilter(int index, GroupFilter groupFilter);
    void removeGroupFilter(GroupFilter groupFilter);
    void updateTabData(int tabIndex, GroupFilter groupFilter);

    void updateUngrouppedTab();
    bool shouldShowUngrouppedTab() const;

private slots:
    INJEQT_SET void setBuddyListMimeDataService(BuddyListMimeDataService *buddyListMimeDataService);
    INJEQT_SET void setChatListMimeDataService(ChatListMimeDataService *chatListMimeDataService);
    INJEQT_SET void setGroupManager(GroupManager *groupManager);
    INJEQT_SET void setIconsManager(IconsManager *iconsManager);
    INJEQT_SET void setInjectedFactory(InjectedFactory *injectedFactory);
    INJEQT_SET void setKaduWindowService(KaduWindowService *kaduWindowService);
    INJEQT_INIT void init();

    void addGroup(Group group);
    void removeGroup(Group group);
    void updateGroup(Group group);

    void currentChangedSlot(int index);

    void addBuddy();
    void deleteGroup();
    void createNewGroup();
    void groupProperties();

    void addToGroup();
    void moveToGroup();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);

public:
    explicit GroupTabBar(QWidget *parent = nullptr);
    virtual ~GroupTabBar();

    void setInitialConfiguration(GroupTabBarConfiguration configuration);
    void setConfiguration(GroupTabBarConfiguration configuration);
    GroupTabBarConfiguration configuration();

    Group groupAt(int index) const;
    GroupFilter groupFilter() const;
    GroupFilter groupFilterAt(int index) const;
    QVector<GroupFilter> groupFilters() const;

signals:
    void currentGroupFilterChanged(const GroupFilter &groupFilter);
};

#endif   // GROUP_TAB_BAR_H
