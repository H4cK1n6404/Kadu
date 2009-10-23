/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QRadioButton>

#include <QtGui/QLineEdit>

#include "accounts/account.h"
#include "accounts/account-manager.h"
#include "configuration/configuration-contact-account-data-manager.h"
#include "contacts/account-data/contact-account-data.h"
#include "contacts/contact-manager.h"
#include "icons-manager.h"
#include "misc/misc.h"
#include "model/actions-proxy-model.h"
#include "model/roles.h"
#include "protocols/protocol.h"
#include "protocols/protocol-factory.h"

#include "contact-general-configuration-widget.h"

ContactGeneralConfigurationWidget::ContactGeneralConfigurationWidget(Contact &contact, QWidget *parent)
	: QWidget(parent), CurrentContact(contact)
{
	setAttribute(Qt::WA_DeleteOnClose);

	createGui();
}

ContactGeneralConfigurationWidget::~ContactGeneralConfigurationWidget()
{
}

void ContactGeneralConfigurationWidget::createGui()
{
	QGridLayout *layout = new QGridLayout(this);
	layout->setColumnMinimumWidth(0, 10);
	layout->setColumnMinimumWidth(1, 10);
	layout->setColumnMinimumWidth(4, 20);
	layout->setColumnMinimumWidth(5, 100);
	layout->setColumnMinimumWidth(8, 20);
	layout->setColumnStretch(3, 10);
	layout->setColumnStretch(6, 2);

	int row = 0;
	layout->setRowStretch(row++, 1); 
	
	QLabel *numberLabel = new QLabel(tr("Visible Name") + ":", this);
	layout->addWidget(numberLabel, 2, 2, 1, 1);
	DisplayEdit = new QLineEdit(this);
	DisplayEdit->setText(CurrentContact.display());
	layout->addWidget(DisplayEdit, 2, 3, 1, 1);

	QWidget *photoWidget = new QWidget;
	QVBoxLayout *photoLayout = new QVBoxLayout(photoWidget);
	photoLayout->setSpacing(2);

	QLabel *photoLabel = new QLabel(this);
	QPixmap photoPixmap = QPixmap(CurrentContact.accountDatas().at(0)->avatar().pixmap());
	photoLabel->setPixmap(photoPixmap);
	photoLayout->addWidget(photoLabel);

	QPushButton *changePhotoButton = new QPushButton(tr("Change Icon..."));
	photoLayout->addWidget(changePhotoButton);

	layout->addWidget(photoWidget, 1, 6, 3, 1);

	QGroupBox *accountsBox = new QGroupBox(tr("Merged Contact Accounts"));
	AccountsLayout = new QGridLayout(accountsBox);
	AccountsLayout->setColumnMinimumWidth(2, 20);
	AccountsLayout->setColumnStretch(0, 1);
	AccountsLayout->setColumnStretch(1, 1);
	AccountsLayout->setColumnStretch(5, 2);
	AccountsLayout->setColumnStretch(6, 2);
	row = 0;

	QLabel *defaultContactLabel = new QLabel(tr("Default Contact") + ":");

	DefaultAccountCombo = new QComboBox(this);
	AccountDataModel = new ContactAccountDataModel(CurrentContact, DefaultAccountCombo);
//	ContactsModelProxy *AccountComboProxyModel = new ContactsModelProxy(DefaultContactCombo);
//	AccountComboProxyModel->setSourceModel(AccountComboModel);
	
// 		ActionsProxyModel::ModelActionList accountsModelBeforeActions;
// 	accountsModelBeforeActions.append(qMakePair<QString, QString>(tr(" - Select merged account - "), ""));
// 	ActionsProxyModel *accountsProxyModel = new ActionsProxyModel(accountsModelBeforeActions,
// 			ActionsProxyModel::ModelActionList(), DefaultContactCombo);
	//accountsProxyModel->setSourceModel(AccountComboProxyModel);
	
	DefaultAccountCombo->setModel(AccountDataModel);
	DefaultAccountCombo->setModelColumn(1); // use long account name
	
	DefaultAccountCombo->setDisabled(CurrentContact.accountDatas().count() <= 1);
	QLabel *defaultContactNoticeLabel = new QLabel(tr("Chat messages will be sent to this username when you select the name from the buddy list"));
	AccountsLayout->addWidget(defaultContactLabel, row, 0, 1, 1);
	AccountsLayout->addWidget(DefaultAccountCombo, row++, 1, 1, 4);
	AccountsLayout->addWidget(defaultContactNoticeLabel, row++, 1, 1, 5);

	QWidget *contactsWidget = new QWidget(this);
	ContactsLayout = new QGridLayout(contactsWidget);
	AccountsLayout->setColumnStretch(0, 2);
	AccountsLayout->setColumnStretch(2, 2);
	
	foreach (ContactAccountData *data, CurrentContact.accountDatas())
	{
		DefaultAccountCombo->addItem(data->id());
		addAccountDataRow(data);
	}

	AccountsLayout->addWidget(contactsWidget, row++, 0, 1, 5);

	QPushButton *addContactButton = new QPushButton(tr("Add Contact..."), this);
	connect(addContactButton, SIGNAL(clicked()), this, SLOT(addAccountDataRow()));
	QPushButton *setOrderButton = new QPushButton(tr("Set Order..."), this);
	setOrderButton->setDisabled(CurrentContact.accountDatas().count() <= 1);
	connect(setOrderButton, SIGNAL(clicked()), this, SLOT(showOrderDialog()));

	AccountsLayout->addWidget(addContactButton, row, 0, 1, 1);
	AccountsLayout->addWidget(setOrderButton, row, 1, 1, 1);

	layout->addWidget(accountsBox, 4, 2, 2, 6);

	QGroupBox *communicationBox = new QGroupBox(tr("Communication Information"));
	QGridLayout *communicationLayout = new QGridLayout(communicationBox);
	communicationLayout->setColumnStretch(0, 1);
	communicationLayout->setColumnStretch(1, 3);
	communicationLayout->setColumnStretch(2, 3);

	row = 0;

	QHBoxLayout *phoneLayout = new QHBoxLayout;
	QLabel *phoneLabel = new QLabel(tr("Phone") + ":");
	PhoneEdit = new QLineEdit(this);
	PhoneEdit->setText(CurrentContact.homePhone());
	communicationLayout->addWidget(phoneLabel, row, 0, 1, 1);
	communicationLayout->addWidget(PhoneEdit, row++, 1, 1, 1);

	QHBoxLayout *mobileLayout = new QHBoxLayout;
	QLabel *mobileLabel = new QLabel(tr("Mobile") + ":");
	MobileEdit = new QLineEdit(this);
	MobileEdit->setText(CurrentContact.mobile());
	communicationLayout->addWidget(mobileLabel, row, 0, 1, 1);
	communicationLayout->addWidget(MobileEdit, row++, 1, 1, 1);

	QHBoxLayout *emailLayout = new QHBoxLayout;
	QLabel *emailLabel = new QLabel(tr("E-Mail") + ":");
	EmailEdit = new QLineEdit(this);
	EmailEdit->setText(CurrentContact.email());
	communicationLayout->addWidget(emailLabel, row, 0, 1, 1);
	communicationLayout->addWidget(EmailEdit, row++, 1, 1, 1);

	QHBoxLayout *websiteLayout = new QHBoxLayout;
	QLabel *websiteLabel = new QLabel(tr("Website") + ":");
	WebsiteEdit = new QLineEdit(this);
	WebsiteEdit->setText(CurrentContact.website());
	communicationLayout->addWidget(websiteLabel, row, 0, 1, 1);
	communicationLayout->addWidget(WebsiteEdit, row++, 1, 1, 1);

	layout->addWidget(communicationBox, 6, 2, 2, 6);
	layout->setRowStretch(8, 100);
}

void ContactGeneralConfigurationWidget::addAccountDataRow(ContactAccountData *data)
{
	int row = ContactsLayout->rowCount();

	QWidget *accountRow = new QWidget(this);
	QGridLayout *accountRowLayout = new QGridLayout(accountRow);
	accountRowLayout->setColumnMinimumWidth(0, 100);
	accountRowLayout->setColumnMinimumWidth(2, 100);
	accountRowLayout->setColumnStretch(0, 5);
	accountRowLayout->setColumnStretch(2, 5);
	accountRowLayout->setColumnStretch(3, 2);

	QLineEdit *contactLineEdit = new QLineEdit(accountRow);
	QLabel *inLabel = new QLabel(tr("in"), accountRow);
	QComboBox *accountsCombo = new QComboBox(accountRow);
	QPushButton *unmergeButton = new QPushButton(IconsManager::instance()->loadIcon("CloseWindowButton"), tr("Unmerge contact..."), accountRow);
	unmergeButton->setDisabled(ContactsAccounts.count() == 0 && CurrentContact.accountDatas().count() <= 1);
	connect(unmergeButton, SIGNAL(clicked(bool)), accountRow, SLOT(hide()));
	connect(unmergeButton, SIGNAL(clicked(bool)), contactLineEdit, SLOT(clear()));

	accountRowLayout->addWidget(contactLineEdit, 0, 0, 1, 1);
	accountRowLayout->addWidget(inLabel, 0, 1, 1, 1);
	accountRowLayout->addWidget(accountsCombo, 0, 2, 1, 1);
	accountRowLayout->addWidget(unmergeButton, 0, 3, 1, 1);

	ContactsIds.append(contactLineEdit);
	ContactsAccounts.append(accountsCombo);

	if (!data)
		accountsCombo->addItem("-" + tr("Select a Network") + "-");
	foreach (Account *account, AccountManager::instance()->accounts())
	{
		accountsCombo->addItem(
		    account->protocol()->icon(), 
		    account->protocol()->protocolFactory()->displayName() + " (" + account->id() + ")",
		    account->uuid().toString()
		);
	}
	if (data)
		accountsCombo->setCurrentIndex(accountsCombo->findData(data->account()->uuid().toString()));

	ContactsLayout->addWidget(accountRow, row, 0, 1, 6);

	if (data)
		contactLineEdit->setText(data->id());
}

void ContactGeneralConfigurationWidget::saveConfiguration()
{
	CurrentContact.setDisplay(DisplayEdit->text());
	CurrentContact.setHomePhone(PhoneEdit->text());
	CurrentContact.setMobile(MobileEdit->text());
	CurrentContact.setEmail(EmailEdit->text());
	CurrentContact.setWebsite(WebsiteEdit->text());

	for (int i = 0; i < ContactsAccounts.count(); i++)
	{
		if (ContactsAccounts.at(i)->itemData(ContactsAccounts.at(i)->currentIndex()).toString().isEmpty())
			break;
		Account *account = AccountManager::instance()->byUuid(QUuid(ContactsAccounts.at(i)->itemData(ContactsAccounts.at(i)->currentIndex()).toString()));
		QString contactId = ContactsIds.at(i)->text();

		if (CurrentContact.hasAccountData(account))
		{
			if (!contactId.isEmpty()/* && account->protocol()->validateId(ContactsIds.at(i)->text())*/)
			{
				CurrentContact.accountData(account)->setId(contactId);
			}
			else
				CurrentContact.removeAccountData(account);
		}
		else
		{
			foreach (ContactAccountData *accountData, CurrentContact.accountDatas())
					if (accountData->id() == contactId) // check if user has only changed account for previous existing ID
						CurrentContact.removeAccountData(accountData->account()); // if so, remove old CAD, otherwise there will appear 2 identical contacts with different accounts
			ContactAccountData *cad = account->protocol()->protocolFactory()
				->newContactAccountData(account, CurrentContact, contactId);
			CurrentContact.addAccountData(cad);
		}
	}
}

void ContactGeneralConfigurationWidget::showOrderDialog()
{
	OrderDialog = new QDialog(this);
	OrderDialog->setAttribute(Qt::WA_DeleteOnClose);
	OrderDialog->setWindowTitle(tr("Set Order"));
	OrderDialog->resize(300, 200);

	QGridLayout *layout = new QGridLayout(OrderDialog);
	layout->setColumnMinimumWidth(0, 10);
	layout->setColumnMinimumWidth(1, 10);
	layout->setColumnMinimumWidth(4, 20);
	layout->setColumnMinimumWidth(5, 100);
	layout->setColumnMinimumWidth(8, 20);
	layout->setColumnStretch(3, 10);
	layout->setColumnStretch(6, 2);

	int row = 0;

	QTreeView *orderView = new QTreeView(OrderDialog);
	layout->addWidget(orderView, row++, 1, 1, 1);

	QDialogButtonBox *buttons = new QDialogButtonBox(Qt::Horizontal, OrderDialog);

	QPushButton *cancelButton = new QPushButton(IconsManager::instance()->loadIcon("CloseWindowButton"), tr("Cancel"), OrderDialog);
	buttons->addButton(cancelButton, QDialogButtonBox::RejectRole);
	QPushButton *saveButton = new QPushButton(IconsManager::instance()->loadIcon("OkWindowButton"), tr("Save"), OrderDialog);
	buttons->addButton(saveButton, QDialogButtonBox::AcceptRole);

	connect(saveButton, SIGNAL(clicked(bool)), this, SLOT(updateOrderAndClose()));
	connect(cancelButton, SIGNAL(clicked(bool)), OrderDialog, SLOT(close()));

	layout->addWidget(buttons, row, 1, 1, 1);

	OrderDialog->show();
}

void ContactGeneralConfigurationWidget::updateOrder()
{
	ContactManager::instance()->blockUpdatedSignal(CurrentContact);

// 	ContactTab->saveConfiguration();
// 	GroupsTab->saveConfiguration(); 
// 	OptionsTab->saveConfiguration(); 

	ContactManager::instance()->unblockUpdatedSignal(CurrentContact);
}

void ContactGeneralConfigurationWidget::updateOrderAndClose()
{
	updateOrder();
	OrderDialog->close();
}

