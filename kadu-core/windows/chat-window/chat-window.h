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

#pragma once

#include "chat/chat.h"
#include "configuration/configuration-aware-object.h"
#include "exports.h"
#include "os/generic/compositing-aware-object.h"
#include "os/generic/desktop-aware-object.h"

#include <QtCore/QPointer>
#include <QtWidgets/QWidget>
#include <injeqt/injeqt.h>

class ChatConfigurationHolder;
class ChatWidget;
class Chat;
class Configuration;
class IconsManager;
class InjectedFactory;

class KADUAPI ChatWindow : public QWidget, ConfigurationAwareObject, CompositingAwareObject, DesktopAwareObject
{
    Q_OBJECT

public:
    explicit ChatWindow(Chat chat, QWidget *parent = nullptr);
    virtual ~ChatWindow();

    Chat chat() const;
    ChatWidget *chatWidget() const
    {
        return m_chatWidget;
    }

    bool isChatWidgetActive(const ChatWidget *chatWidget);

    void setWindowTitle(QString title);

signals:
    void activated(ChatWindow *chatWindow);
    void windowDestroyed(ChatWindow *chatWindow);

protected:
    virtual void closeEvent(QCloseEvent *e) override;
    virtual void changeEvent(QEvent *event) override;

    virtual void configurationUpdated() override;

    virtual void compositingEnabled() override;
    virtual void compositingDisabled() override;

private:
    QPointer<ChatConfigurationHolder> m_chatConfigurationHolder;
    QPointer<Configuration> m_configuration;
    QPointer<IconsManager> m_iconsManager;
    QPointer<InjectedFactory> m_injectedFactory;

    Chat m_chat;
    ChatWidget *m_chatWidget;

    QRect defaultGeometry() const;

private slots:
    INJEQT_SET void setChatConfigurationHolder(ChatConfigurationHolder *chatConfigurationHolder);
    INJEQT_SET void setConfiguration(Configuration *configuration);
    INJEQT_SET void setIconsManager(IconsManager *iconsManager);
    INJEQT_SET void setInjectedFactory(InjectedFactory *injectedFactory);
    INJEQT_INIT void init();

    void updateTitle();
};
