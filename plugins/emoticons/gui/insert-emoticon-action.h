/*
 * %kadu copyright begin%
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

#ifndef INSERT_EMOTICON_ACTION_H
#define INSERT_EMOTICON_ACTION_H

#include "actions/action-description.h"
#include "configuration/emoticon-configuration.h"
#include "emoticon.h"
#include "injeqt-type-roles.h"

/**
 * @addtogroup Emoticons
 * @{
 */

/**
 * @class InsertEmoticonAction
 * @short Action for inserting emoticons into chat edit boxes.
 */
class InsertEmoticonAction : public ActionDescription
{
    Q_OBJECT
    INJEQT_TYPE_ROLE(ACTION)

    EmoticonConfiguration Configuration;

protected:
    virtual void actionInstanceCreated(Action *action);
    virtual void updateActionState(Action *action);

public:
    /**
     * @short Create new instance of InsertEmoticonAction.
     * @param parent QObject parent
     */
    Q_INVOKABLE explicit InsertEmoticonAction(QObject *parent = nullptr);
    virtual ~InsertEmoticonAction();

    virtual void actionTriggered(QAction *sender, bool toggled);

    /**
     * @short Configure instance of InsertEmoticonAction.
     * @param configuration new configuration
     *
     * This configuration is used to get list of emoticons to pass to EmoticonSelector widget.
     */
    void setConfiguration(const EmoticonConfiguration &configuration);
};

/**
 * @}
 */

#endif   // INSERT_EMOTICON_ACTION_H
