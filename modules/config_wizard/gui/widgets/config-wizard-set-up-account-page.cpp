/*
 * %kadu copyright begin%
 * Copyright 2010 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include <QtGui/QCheckBox>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>

#include "accounts/account-manager.h"
#include "gui/widgets/account-add-widget.h"
#include "gui/widgets/account-create-widget.h"
#include "gui/windows/your-accounts.h"
#include "protocols/protocol-factory.h"

#include "config-wizard-set-up-account-page.h"

ConfigWizardSetUpAccountPage::ConfigWizardSetUpAccountPage(QWidget *parent) :
		ConfigWizardPage(parent), AddAccountWidget(0), CreateAccountWidget(0)
{
	setDescription("<p>Please enter your account data.</p><p>Go back if you want to select a different Account Setup option.</p>");

	createGui();
}

ConfigWizardSetUpAccountPage::~ConfigWizardSetUpAccountPage()
{
}

void ConfigWizardSetUpAccountPage::createGui()
{
	formLayout()->addRow(new QLabel(tr("<h3>Account Setup</h3>"), this));
}

bool ConfigWizardSetUpAccountPage::isComplete() const
{
	if (CreateAccountWidget)
		return StateChangedDataValid == CreateAccountWidget->state();
	if (AddAccountWidget)
		return StateChangedDataValid == AddAccountWidget->state();

	return true;
}

void ConfigWizardSetUpAccountPage::initializePage()
{
	ProtocolFactory *pf = field("choose-network.protocol-factory").value<ProtocolFactory *>();
	if (!pf)
		return;

	if (field("choose-network.new").toBool())
	{
		CreateAccountWidget = pf->newCreateAccountWidget(this);
		formLayout()->addRow(QString(), CreateAccountWidget);

		connect(CreateAccountWidget, SIGNAL(stateChanged(ModalConfigurationWidgetState)), this, SIGNAL(completeChanged()));
	} else if (field("choose-network.existing").toBool())
	{
		AddAccountWidget = pf->newAddAccountWidget(this);
		formLayout()->addRow(QString(), AddAccountWidget);

		connect(CreateAccountWidget, SIGNAL(stateChanged(ModalConfigurationWidgetState)), this, SIGNAL(completeChanged()));
	}
}

void ConfigWizardSetUpAccountPage::cleanupPage()
{
	delete AddAccountWidget;
	AddAccountWidget = 0;
	delete CreateAccountWidget;
	CreateAccountWidget = 0;

	QWizardPage::cleanupPage();
}

void ConfigWizardSetUpAccountPage::acceptPage()
{
}
