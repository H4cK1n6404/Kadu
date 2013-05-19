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

#include <stdio.h>
#include <qglobal.h>

extern "C" {
#	include <libotr/userstate.h>
}

#include "accounts/account.h"
#include "chat/chat.h"
#include "contacts/contact-manager.h"
#include "formatted-string/formatted-string-factory.h"
#include "gui/widgets/chat-widget.h"
#include "gui/widgets/chat-widget-manager.h"
#include "message/message-manager.h"
#include "protocols/protocol.h"
#include "protocols/protocol-factory.h"
#include "protocols/services/chat-service.h"

#include "encryption-ng-otr-notifier.h"
#include "encryption-ng-otr-op-data.h"
#include "encryption-ng-otr-policy.h"
#include "encryption-ng-otr-policy-account-store.h"
#include "encryption-ng-otr-private-key-service.h"

#include "encryption-ng-otr-app-ops-wrapper.h"

OtrlPolicy kadu_enomf_policy(void *opdata, ConnContext *context)
{
	Q_UNUSED(context);

	EncryptionNgOtrOpData *ngOtrOpData = static_cast<EncryptionNgOtrOpData *>(opdata);
	return ngOtrOpData->appOpsWrapper()->policy(ngOtrOpData);
}

void kadu_enomf_create_privkey(void *opdata, const char *accountname, const char *protocol)
{
	Q_UNUSED(accountname);
	Q_UNUSED(protocol);

	EncryptionNgOtrOpData *ngOtrOpData = static_cast<EncryptionNgOtrOpData *>(opdata);
	ngOtrOpData->appOpsWrapper()->createPrivateKey(ngOtrOpData);
}

int kadu_enomf_is_logged_in(void *opdata, const char *accountname, const char *protocol, const char *recipient)
{
	Q_UNUSED(accountname);
	Q_UNUSED(protocol);

	EncryptionNgOtrOpData *ngOtrOpData = static_cast<EncryptionNgOtrOpData *>(opdata);
	return (int)ngOtrOpData->appOpsWrapper()->isLoggedIn(ngOtrOpData, recipient);
}

void kadu_enomf_inject_message(void *opdata, const char *accountname, const char *protocol, const char *recipient, const char *message)
{
	Q_UNUSED(accountname);
	Q_UNUSED(protocol);
	Q_UNUSED(recipient);

	EncryptionNgOtrOpData *ngOtrOpData = static_cast<EncryptionNgOtrOpData *>(opdata);
	ngOtrOpData->appOpsWrapper()->injectMessage(ngOtrOpData, QString::fromUtf8(message));
}

void kadu_enomf_notify(void *opdata, OtrlNotifyLevel level, const char *accountname, const char *protocol, const char *username,
					   const char *title, const char *primary, const char *secondary)
{
	Q_UNUSED(level);
	Q_UNUSED(accountname);
	Q_UNUSED(protocol);
	Q_UNUSED(username);

	EncryptionNgOtrOpData *ngOtrOpData = static_cast<EncryptionNgOtrOpData *>(opdata);
	ngOtrOpData->appOpsWrapper()->notify(ngOtrOpData, QString::fromUtf8(title), QString::fromUtf8(primary), QString::fromUtf8(secondary));
}

int kadu_enomf_display_otr_message(void *opdata, const char *accountname, const char *protocol, const char *username, const char *msg)
{
	Q_UNUSED(accountname);
	Q_UNUSED(protocol);
	Q_UNUSED(username);

	EncryptionNgOtrOpData *ngOtrOpData = static_cast<EncryptionNgOtrOpData *>(opdata);
	if (ngOtrOpData->appOpsWrapper()->displayOtrMessage(ngOtrOpData, QString::fromUtf8(msg)))
		return 0;
	else
		return -1;
}

void kadu_enomf_update_context_list(void *opdata)
{
	printf("kadu_enomf_update_context_list %p\n", opdata);
}

const char * kadu_enomf_protocol_name(void *opdata, const char *protocol)
{
	printf("kadu_enomf_protocol_name %p %s\n", opdata, protocol);
	return protocol;
}

void kadu_enomf_protocol_name_free(void *opdata, const char *protocol_name)
{
	printf("kadu_enomf_protocol_name_free %p %s\n", opdata, protocol_name);
}

void kadu_enomf_new_fingerprint(void *opdata, OtrlUserState us, const char *accountname, const char *protocol,
								const char *username, unsigned char fingerprint[20])
{
	Q_UNUSED(fingerprint);
	printf("kadu_enomf_new_fingerprint %p %p %s %s %s\n", opdata, us->context_root, accountname, protocol, username);
}

void kadu_enomf_write_fingerprints(void *opdata)
{
	printf("kadu_enomf_write_fingerprints %p\n", opdata);
}

void kadu_enomf_gone_secure(void *opdata, ConnContext *context)
{
	Q_UNUSED(context);

	EncryptionNgOtrOpData *ngOtrOpData = static_cast<EncryptionNgOtrOpData *>(opdata);
	ngOtrOpData->appOpsWrapper()->goneSecure(ngOtrOpData);
}

void kadu_enomf_gone_insecure(void *opdata, ConnContext *context)
{
	Q_UNUSED(context);

	EncryptionNgOtrOpData *ngOtrOpData = static_cast<EncryptionNgOtrOpData *>(opdata);
	ngOtrOpData->appOpsWrapper()->goneInsecure(ngOtrOpData);
}

void kadu_enomf_still_secure(void *opdata, ConnContext *context, int is_reply)
{
	Q_UNUSED(context);
	Q_UNUSED(is_reply);

	EncryptionNgOtrOpData *ngOtrOpData = static_cast<EncryptionNgOtrOpData *>(opdata);
	ngOtrOpData->appOpsWrapper()->stillSecure(ngOtrOpData);
}

void kadu_enomf_log_message(void *opdata, const char *message)
{
	printf("kadu_enomf_log_message %p %s\n", opdata, message);
}

int kadu_enomf_max_message_size(void *opdata, ConnContext *context)
{
	Q_UNUSED(context);

	EncryptionNgOtrOpData *ngOtrOpData = static_cast<EncryptionNgOtrOpData *>(opdata);
	return ngOtrOpData->appOpsWrapper()->maxMessageSize(ngOtrOpData);
}

const char * kadu_enomf_account_name(void *opdata, const char *account, const char *protocol)
{
	printf("kadu_enomf_account_name %p %s %s\n", opdata, account, protocol);
	return protocol;
}

void kadu_enomf_account_name_free(void *opdata, const char *account_name)
{
	printf("kadu_enomf_account_name_free %p %s\n", opdata, account_name);
}

EncryptionNgOtrAppOpsWrapper::EncryptionNgOtrAppOpsWrapper()
{
	Ops.policy = kadu_enomf_policy;
	Ops.create_privkey = kadu_enomf_create_privkey;
	Ops.is_logged_in = kadu_enomf_is_logged_in;
	Ops.inject_message = kadu_enomf_inject_message;
	Ops.notify = kadu_enomf_notify;
	Ops.display_otr_message = kadu_enomf_display_otr_message;
	Ops.update_context_list = kadu_enomf_update_context_list;
	Ops.protocol_name = kadu_enomf_protocol_name;
	Ops.protocol_name_free = kadu_enomf_protocol_name_free;
	Ops.new_fingerprint = kadu_enomf_new_fingerprint;
	Ops.write_fingerprints = kadu_enomf_write_fingerprints;
	Ops.gone_secure = kadu_enomf_gone_secure;
	Ops.gone_insecure = kadu_enomf_gone_insecure;
	Ops.still_secure = kadu_enomf_still_secure;
	Ops.log_message = kadu_enomf_log_message;
	Ops.max_message_size = kadu_enomf_max_message_size;
	Ops.account_name = kadu_enomf_account_name;
	Ops.account_name_free = kadu_enomf_account_name_free;
}

EncryptionNgOtrAppOpsWrapper::~EncryptionNgOtrAppOpsWrapper()
{
}

void EncryptionNgOtrAppOpsWrapper::setFormattedStringFactory(FormattedStringFactory *formattedStringFactory)
{
	MyFormattedStringFactory = formattedStringFactory;
}

const OtrlMessageAppOps * EncryptionNgOtrAppOpsWrapper::ops() const
{
	return &Ops;
}

OtrlPolicy EncryptionNgOtrAppOpsWrapper::policy(EncryptionNgOtrOpData *ngOtrOpData)
{
	Account account = ngOtrOpData->message().messageChat().chatAccount();
	EncryptionNgOtrPolicy policy = EncryptionNgOtrPolicyAccountStore::loadPolicyFromAccount(account);

	return policy.toOtrPolicy();
}

void EncryptionNgOtrAppOpsWrapper::createPrivateKey(EncryptionNgOtrOpData *ngOtrOpData)
{
	Account account = ngOtrOpData->message().messageChat().chatAccount();
	ngOtrOpData->privateKeyService()->createPrivateKey(account);
}

EncryptionNgOtrAppOpsWrapper::IsLoggedInStatus EncryptionNgOtrAppOpsWrapper::isLoggedIn(EncryptionNgOtrOpData *ngOtrOpData, const QString &contactId)
{
	Account account = ngOtrOpData->message().messageChat().chatAccount();
	Contact contact = ContactManager::instance()->byId(account, contactId, ActionReturnNull);

	if (!contact)
		return NotSure;

	if (contact.currentStatus().isDisconnected())
		return NotLoggedIn;
	else
		return LoggedIn;
}

void EncryptionNgOtrAppOpsWrapper::injectMessage(EncryptionNgOtrOpData *ngOtrOpData, const QString &messageContent)
{
	Chat chat = ngOtrOpData->message().messageChat();
	MessageManager::instance()->sendMessage(chat, messageContent, true);
}

void EncryptionNgOtrAppOpsWrapper::notify(EncryptionNgOtrOpData *ngOtrOpData, const QString &title, const QString &primary, const QString &secondary)
{
	Chat chat = ngOtrOpData->message().messageChat();
	ngOtrOpData->notifier()->notifyGeneric(chat, QString("<b>%1<b/>: %2").arg(title).arg(primary), secondary);
}

bool EncryptionNgOtrAppOpsWrapper::displayOtrMessage(EncryptionNgOtrOpData *ngOtrOpData, const QString &messageContent)
{
	if (!MyFormattedStringFactory)
		return false;

	Chat chat = ngOtrOpData->message().messageChat();
	ChatWidget *chatWidget = ChatWidgetManager::instance()->byChat(chat, false);
	if (!chatWidget)
		return false;

	FormattedString *messageString = MyFormattedStringFactory.data()->fromText(messageContent);
	chatWidget->appendSystemMessage(messageString);
	return true;
}

void EncryptionNgOtrAppOpsWrapper::goneSecure(EncryptionNgOtrOpData *ngOtrOpData)
{
	Chat chat = ngOtrOpData->message().messageChat();
	ngOtrOpData->notifier()->notifyGoneSecure(chat);
}

void EncryptionNgOtrAppOpsWrapper::goneInsecure(EncryptionNgOtrOpData *ngOtrOpData)
{
	Chat chat = ngOtrOpData->message().messageChat();
	ngOtrOpData->notifier()->notifyGoneInsecure(chat);
}

void EncryptionNgOtrAppOpsWrapper::stillSecure(EncryptionNgOtrOpData *ngOtrOpData)
{
	Chat chat = ngOtrOpData->message().messageChat();
	ngOtrOpData->notifier()->notifyStillSecure(chat);
}

int EncryptionNgOtrAppOpsWrapper::maxMessageSize(EncryptionNgOtrOpData *ngOtrOpData)
{
	Account account = ngOtrOpData->message().messageChat().chatAccount();
	Protocol *protocolHandler = account.protocolHandler();
	if (!protocolHandler)
		return 0;
	ChatService *chatService = protocolHandler->chatService();
	if (!chatService)
		return 0;
	return chatService->maxMessageLength();
}
