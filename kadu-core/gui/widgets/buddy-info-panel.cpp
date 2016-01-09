/*
 * %kadu copyright begin%
 * Copyright 2010, 2011 Piotr Galiszewski (piotr.galiszewski@kadu.im)
 * Copyright 2010, 2011 Piotr Dąbrowski (ultr@ultr.pl)
 * Copyright 2009 Bartłomiej Zimoń (uzi18@o2.pl)
 * Copyright 2010, 2011, 2012, 2013, 2014 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2009, 2010, 2011, 2012, 2013, 2014, 2015 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include <QtWebKitWidgets/QWebFrame>

#include "accounts/account.h"
#include "avatars/avatar-manager.h"
#include "avatars/avatar.h"
#include "buddies/buddy-preferred-manager.h"
#include "buddies/buddy.h"
#include "configuration/configuration.h"
#include "configuration/deprecated-configuration-api.h"
#include "contacts/contact-manager.h"
#include "core/core.h"
#include "dom/dom-processor-service.h"
#include "misc/syntax-list.h"
#include "parser/parser.h"
#include "url-handlers/url-handler-manager.h"
#include "debug.h"

#include "buddy-info-panel.h"

BuddyInfoPanel::BuddyInfoPanel(QWidget *parent) : KaduWebView(parent)
{
	QPalette p = palette();
	p.setBrush(QPalette::Base, Qt::transparent);
	page()->setPalette(p);
	setAttribute(Qt::WA_OpaquePaintEvent, false);

	page()->currentFrame()->evaluateJavaScript(
		"XMLHttpRequest.prototype.open = function() { return false; };"
		"XMLHttpRequest.prototype.send = function() { return false; };"
	);

	connect(BuddyPreferredManager::instance(), SIGNAL(buddyUpdated(Buddy)), this, SLOT(buddyUpdated(Buddy)));
}

BuddyInfoPanel::~BuddyInfoPanel()
{
	disconnect(BuddyPreferredManager::instance(), 0, this, 0);
}

void BuddyInfoPanel::setConfiguration(Configuration *configuration)
{
	m_configuration = configuration;
}

void BuddyInfoPanel::setDomProcessorService(DomProcessorService *domProcessorService)
{
	m_domProcessorService = domProcessorService;
}

void BuddyInfoPanel::init()
{
	configurationUpdated();
}

void BuddyInfoPanel::configurationUpdated()
{
	setUserFont(m_configuration->deprecatedApi()->readFontEntry("Look", "PanelFont").toString(), true);

	update();
}

void BuddyInfoPanel::buddyUpdated(const Buddy &buddy)
{
	if (buddy == Item.toBuddy())
		update();
}

void BuddyInfoPanel::update()
{
	if (!m_configuration)
		return;

	QFont font = m_configuration->deprecatedApi()->readFontEntry("Look", "PanelFont");
	QString fontFamily = font.family();
	QString fontSize;
	if (font.pointSize() > 0)
		fontSize = QString::number(font.pointSize()) + "pt";
	else
		fontSize = QString::number(font.pixelSize()) + "px";

	QString fontStyle = font.italic() ? "italic" : "normal";
	QString fontWeight = font.bold() ? "bold" : "normal";
	QString textDecoration = font.underline() ? "underline" : "none";
	QString fontColor = m_configuration->deprecatedApi()->readColorEntry("Look", "InfoPanelFgColor").name();
	bool backgroundFilled = m_configuration->deprecatedApi()->readBoolEntry("Look", "InfoPanelBgFilled");
	if (backgroundFilled)
		BackgroundColor = m_configuration->deprecatedApi()->readColorEntry("Look", "InfoPanelBgColor").name();
	else
		BackgroundColor = "transparent";

	Template = QString(
		"<html>"
		"	<head>"
		"		<style type='text/css'>"
		"		html {"
		"			color: %1;"
		"			font: %2 %3 %4 %5;"
		"			text-decoration: %6;"
		"			margin: 0;"
		"			padding: 0;"
		"			background-color: %7;"
		"		}"
		"		div {"
		"			color: %1;"
		"			font: %2 %3 %4 %5;"
		"			text-decoration: %6;"
		"			margin: 0;"
		"			padding: 0;"
		"			background-color: %7;"
		"		}"
		"		table {"
		"			color: %1;"
		"			font: %2 %3 %4 %5;"
		"			text-decoration: %6;"
		"		}"
		"		</style>"
		"	</head>"
		"	<body>"
		"		%8"
		"	</body>"
		"</html>"
		).arg(fontColor, fontStyle, fontWeight, fontSize, fontFamily, textDecoration, BackgroundColor, "%1");

	QString syntaxFile = m_configuration->deprecatedApi()->readEntry("Look", "InfoPanelSyntaxFile", "ultr");
	if (syntaxFile == "default")
	{
		syntaxFile = "Old Default";
		m_configuration->deprecatedApi()->writeEntry("Look", "InfoPanelSyntaxFile", syntaxFile);
	}

	Syntax = SyntaxList::readSyntax("infopanel", syntaxFile,
		"<table><tr><td><img width=\"32\" height=\"32\" align=\"left\" valign=\"top\" src=\"@{x-office-address-book:32x32}\"></td><td> "
		"<div align=\"left\"> [<b>%a</b>][ (%u)] [<br>tel.: %m][<br>IP: %i]</div></td></tr></table> <hr> <b>%s</b> [<br>%d]");
	Syntax = Syntax.remove("file:///");
	displayItem(Item);

	if (m_configuration->deprecatedApi()->readBoolEntry("Look", "PanelVerticalScrollbar"))
		page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAsNeeded);
	else
		page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
}

void BuddyInfoPanel::connectItem()
{
	Buddy buddy = Item.toBuddy();
	if (buddy)
	{
		connect(buddy, SIGNAL(updated()), this, SLOT(update()));
		if (buddy.buddyAvatar())
			connect(buddy.buddyAvatar(), SIGNAL(updated()), this, SLOT(update()));
	}

	Contact contact = Item.toContact();
	if (contact)
	{
		connect(contact, SIGNAL(updated()), this, SLOT(update()));
		auto avatar = AvatarManager::instance()->byContact(contact, ActionReturnNull);
		if (avatar)
			connect(avatar, SIGNAL(updated()), this, SLOT(update()));
	}
}

void BuddyInfoPanel::disconnectItem()
{
	Buddy buddy = Item.toBuddy();
	if (buddy)
	{
		disconnect(buddy, 0, this, 0);
		if (buddy.buddyAvatar())
			disconnect(buddy.buddyAvatar(), 0, this, 0);
	}

	Contact contact = Item.toContact();
	if (contact)
	{
		disconnect(contact, 0, this, 0);
		auto avatar = AvatarManager::instance()->byContact(contact, ActionReturnNull);
		if (avatar)
			disconnect(avatar, 0, this, 0);
	}
}

void BuddyInfoPanel::displayItem(Talkable item)
{
	disconnectItem();
	Item = item;
	connectItem();

	if (!isVisible())
		return;

	if (item.isEmpty())
	{
		setHtml("<body bgcolor=\"" + BackgroundColor + "\"></body>");
		return;
	}

	QDomDocument domDocument;
	domDocument.setContent(Template.arg(Parser::parse(Syntax, item, ParserEscape::HtmlEscape)));

	m_domProcessorService->process(domDocument);

	setHtml(domDocument.toString(0));
}

void BuddyInfoPanel::setVisible(bool visible)
{
	QWidget::setVisible(visible);

	if (visible)
		displayItem(Item);
}

void BuddyInfoPanel::styleFixup(QString &syntax)
{
	syntax = Template.arg(syntax);
}

#include "moc_buddy-info-panel.cpp"
