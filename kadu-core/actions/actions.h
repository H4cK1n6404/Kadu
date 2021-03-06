/*
 * %kadu copyright begin%
 * Copyright 2011 Piotr Galiszewski (piotr.galiszewski@kadu.im)
 * Copyright 2011 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2009, 2010, 2011 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include "exports.h"

#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <injeqt/injeqt.h>

class QAction;

class Action;
class ActionContext;
class ActionDescription;
class SessionService;

/**
 * @addtogroup Actions
 * @{
 */

/**
 * @class Actions
 * @short Manager of all ActionDescription and Action instances.
 *
 * This singleton manager all instanced of ActionDescription inside Kadu. It emits actionCreated signal
 * on each Action instance creation and actionLoaded and actionUloaded when plugins are loaded and unloaded
 * as pluginc can register and unregister new action types.
 */
class KADUAPI Actions : public QObject, public QMap<QString, ActionDescription *>
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit Actions(QObject *parent = nullptr);
    virtual ~Actions();

    /**
     * @short Creates new QAction instance for kadu main window.
     * @param name name of action to create
     * @param context context of action
     * @param parent parent object of action
     *
     * This method creates new instance of given action for given context. Signal actionCreated is emited after
     * creation, no matter what blocking signals state is.
     */
    QAction *createAction(const QString &name, ActionContext *context, QObject *parent);

    /**
     * @short Blocks actionLoaded and actionUnloaded signals.
     *
     * No more actionLoaded and actionUnloaded signals will be emited, until unblockSignals is called.
     */
    void blockSignals()
    {
        BlockSignals = true;
    }

    /**
     * @short Unblocks actionLoaded and actionUnloaded signals.
     *
     * Signals actionLoaded and actionUnloaded can now be emited again.
     */
    void unblockSignals()
    {
        BlockSignals = false;
    }

signals:
    /**
     * @short Signal emited after an action for main kadu window was created.
     * @param action newly created action
     *
     * Signal emited after an action for main kadu window was created.
     */
    void actionCreated(Action *action);

    /**
     * @short Signal emited after an action type was registerd.
     * @param action registered action.
     *
     * Signal emited after an action type was registered. When Actions object is in block signals state, this
     * signal is not emited.
     */
    void actionLoaded(ActionDescription *action);

    /**
     * @short Signal emited after an action type was unregisterd.
     * @param action unregistered action.
     *
     * Signal emited after an action type was unregistered. When Actions object is in block signals state, this
     * signal is not emited.
     */
    void actionUnloaded(ActionDescription *action);

public:
    /**
     * @short Registers ActionDescription class.
     * @param action registered action
     *
     * This method can only by called from ActionDescription instances when they are ready to be registered.
     * Signal actionLoaded is emited if signals are non blocked.
     */
    bool insert(ActionDescription *action);

    /**
     * @short Unregisters an ActionDescription class.
     * @param action unregistered action
     *
     * This method can only by called from ActionDescription instances when they are ready to be unregistered.
     * Signal actionUnloaded is emited if signals are non blocked.
     */
    void remove(ActionDescription *action);

private:
    QPointer<SessionService> m_sessionService;

    bool BlockSignals;

private slots:
    INJEQT_SET void setSessionService(SessionService *sessionService);
};

/**
 * @}
 */
