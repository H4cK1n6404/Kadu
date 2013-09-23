/*
 * %kadu copyright begin%
 * Copyright 2013 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QVBoxLayout>

#include "otr-peer-identity-verification-service.h"
#include "otr-peer-identity-verification-state.h"

#include "otr-peer-identity-verification-progress-page.h"

OtrPeerIdentityVerificationProgressPage::OtrPeerIdentityVerificationProgressPage(const Contact &contact, QWidget *parent) :
		QWizardPage(parent), MyContact(contact)
{
	createGui();
}

OtrPeerIdentityVerificationProgressPage::~OtrPeerIdentityVerificationProgressPage()
{
}

void OtrPeerIdentityVerificationProgressPage::createGui()
{
	setTitle(tr("Verification Progress"));

	QVBoxLayout *layout = new QVBoxLayout(this);

	StateLabel = new QLabel();
	StateProgress = new QProgressBar();
	StateProgress->setMaximum(100);

	layout->addWidget(StateLabel);
	layout->addWidget(StateProgress);
}

void OtrPeerIdentityVerificationProgressPage::setState(const OtrPeerIdentityVerificationState &state)
{
	State = state;
	StateLabel->setText(stateToString(State));
	StateProgress->setValue(State.percentCompleted());
}

QString OtrPeerIdentityVerificationProgressPage::stateToString(const OtrPeerIdentityVerificationState &state)
{
	switch (state.state())
	{
		case OtrPeerIdentityVerificationState::StateNotStarted:
			return tr("Verification not started");
		case OtrPeerIdentityVerificationState::StateInProgress:
			return tr("Verification in progres...");
		case OtrPeerIdentityVerificationState::StateFailed:
			return tr("Verification failed");
		case OtrPeerIdentityVerificationState::StateSucceeded:
			return tr("Verification succeeded");
		default:
			return tr("Unknown");
	}
}

void OtrPeerIdentityVerificationProgressPage::setPeerIdentityVerificationService(OtrPeerIdentityVerificationService *peerIdentityVerificationService)
{
	if (PeerIdentityVerificationService)
		disconnect(PeerIdentityVerificationService.data(), 0, this, 0);
	PeerIdentityVerificationService = peerIdentityVerificationService;
	if (PeerIdentityVerificationService)
		connect(PeerIdentityVerificationService.data(), SIGNAL(contactStateUpdated(Contact,OtrPeerIdentityVerificationState)),
				this, SLOT(contactStateUpdated(Contact,OtrPeerIdentityVerificationState)));
}

void OtrPeerIdentityVerificationProgressPage::contactStateUpdated(const Contact &contact, const OtrPeerIdentityVerificationState &state)
{
	if (contact == MyContact)
		setState(state);
}

int OtrPeerIdentityVerificationProgressPage::nextId() const
{
	return -1;
}

void OtrPeerIdentityVerificationProgressPage::initializePage()
{
	setState(OtrPeerIdentityVerificationState());
}

bool OtrPeerIdentityVerificationProgressPage::validatePage()
{
	if (!PeerIdentityVerificationService)
		return true;

	if (OtrPeerIdentityVerificationState::StateInProgress == State.state() || OtrPeerIdentityVerificationState::StateNotStarted == State.state())
		PeerIdentityVerificationService.data()->cancelVerification(MyContact);

	return true;
}