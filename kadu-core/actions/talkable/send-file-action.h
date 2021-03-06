/*
 * %kadu copyright begin%
 * Copyright 2015 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include "actions/action-description.h"
#include "injeqt-type-roles.h"

#include <QtCore/QPointer>
#include <injeqt/injeqt.h>

class ContactSet;
class FileTransferManager;
class FileTransferStorage;
class Myself;

class SendFileAction : public ActionDescription
{
    Q_OBJECT
    INJEQT_TYPE_ROLE(ACTION)

public:
    Q_INVOKABLE explicit SendFileAction(QObject *parent = nullptr);
    virtual ~SendFileAction();

protected:
    virtual void actionInstanceCreated(Action *action) override;
    virtual void triggered(QWidget *widget, ActionContext *context, bool toggled) override;
    virtual void updateActionState(Action *action) override;

private:
    QPointer<FileTransferManager> m_fileTransferManager;
    QPointer<FileTransferStorage> m_fileTransferStorage;
    QPointer<Myself> m_myself;

    void selectFilesAndSend(const ContactSet &contacts);
    QStringList selectFilesToSend() const;

private slots:
    INJEQT_SET void setFileTransferManager(FileTransferManager *fileTransferManager);
    INJEQT_SET void setFileTransferStorage(FileTransferStorage *fileTransferStorage);
    INJEQT_SET void setMyself(Myself *myself);
};
