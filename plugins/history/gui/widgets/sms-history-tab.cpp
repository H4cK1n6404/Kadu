/*
 * %kadu copyright begin%
 * Copyright 2012 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include <QtGui/QMenu>
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>

#include "buddies/model/buddy-list-model.h"
#include "gui/widgets/chat-messages-view.h"
#include "gui/widgets/filtered-tree-view.h"
#include "gui/widgets/talkable-delegate-configuration.h"
#include "gui/widgets/talkable-tree-view.h"
#include "gui/widgets/timeline-chat-messages-view.h"
#include "icons/kadu-icon.h"
#include "model/roles.h"
#include "model/model-chain.h"
#include "talkable/filter/name-talkable-filter.h"
#include "talkable/model/talkable-proxy-model.h"
#include "talkable/talkable.h"

#include "model/dates-model-item.h"
#include "search/history-search-parameters.h"
#include "storage/history-messages-storage.h"

#include "sms-history-tab.h"

SmsHistoryTab::SmsHistoryTab(QWidget *parent) :
		HistoryTab(false, parent)
{
	CurrentRecipient = Buddy::create();
}

SmsHistoryTab::~SmsHistoryTab()
{
}

void SmsHistoryTab::displaySmsRecipient(const QString& recipient, bool force)
{
	if (!force && CurrentRecipient.mobile() == recipient)
		return;

	Chat smsChat = Chat::create();
	smsChat.setDisplay(recipient);
	timelineView()->messagesView()->setChat(smsChat);

	CurrentRecipient.setMobile(recipient);

	if (historyMessagesStorage())
		setFutureDates(historyMessagesStorage()->dates(Talkable(CurrentRecipient)));
	else
		setDates(QVector<DatesModelItem>());
}

void SmsHistoryTab::clearSmsHistory()
{
	bool removed = false;

	if (!historyMessagesStorage())
		return;

	BuddySet buddies = talkableTree()->actionContext()->buddies();
	foreach (const Buddy &buddy, buddies)
	{
		if (buddy.mobile().isEmpty())
			continue;

		removed = true;
		historyMessagesStorage()->deleteMessages(buddy);
	}

	if (removed)
	{
		updateData();
		displaySmsRecipient(QString(), false);
	}
}

void SmsHistoryTab::currentTalkableChanged(const Talkable &talkable)
{
	if (talkable.isValidBuddy())
		displaySmsRecipient(talkable.toBuddy().mobile(), false);
	else
		displaySmsRecipient(QString(), false);
}

void SmsHistoryTab::modifyTalkablePopupMenu(const QScopedPointer<QMenu> &menu)
{
	if (!menu)
		return;

	menu->addSeparator();
	menu->addAction(KaduIcon("kadu_icons/clear-history").icon(),
			tr("&Clear SMS History"), this, SLOT(clearSmsHistory()));
}

void SmsHistoryTab::displayForDate(const QDate &date)
{
	if (!CurrentRecipient.mobile().isEmpty() && date.isValid() && historyMessagesStorage())
		setFutureMessages(historyMessagesStorage()->messages(CurrentRecipient, date));
	else
		setMessages(QVector<Message>());
}

void SmsHistoryTab::removeEntriesPerDate(const QDate &date)
{
	if (!CurrentRecipient.mobile().isEmpty() && historyMessagesStorage())
	{
		historyMessagesStorage()->deleteMessages(CurrentRecipient, date);
		displaySmsRecipient(CurrentRecipient.mobile(), true);
	}
}

void SmsHistoryTab::updateData()
{
	setMessages(QVector<Message>());

	if (!historyMessagesStorage())
	{
		setTalkables(QVector<Talkable>());
		displaySmsRecipient(QString(), false);
		return;
	}

	setFutureTalkables(historyMessagesStorage()->talkables());
}
