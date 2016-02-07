/*
 * %kadu copyright begin%
 * Copyright 2011 Piotr Galiszewski (piotr.galiszewski@kadu.im)
 * Copyright 2012 Wojciech Treter (juzefwt@gmail.com)
 * Copyright 2011, 2012, 2013 Bartosz Brachaczek (b.brachaczek@gmail.com)
 * Copyright 2011, 2013, 2014 Rafał Przemysław Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include <QtCore/QProcess>
#include <QtGui/QTextDocument>

#include "configuration/configuration.h"
#include "configuration/deprecated-configuration-api.h"
#include "core/core.h"
#include "core/core.h"
#include "notification/notification-manager.h"
#include "notification/notification.h"
#include "parser/parser.h"
#include "debug.h"
#include "speech-configuration-widget.h"

#include "speech.h"

/**
 * @ingroup speech
 * @{
 */

bool isFemale(const QString &s)
{
	return s.endsWith('a', Qt::CaseInsensitive);
}

Speech::Speech(QObject *parent) :
		QObject{parent},
		Notifier("Speech", QT_TRANSLATE_NOOP("@default", "Read a text"), KaduIcon()), lastSpeech()
{
}

Speech::~Speech()
{
}

void Speech::setConfiguration(Configuration *configuration)
{
	m_configuration = configuration;
}

void Speech::setParser(Parser *parser)
{
	m_parser = parser;
}

void Speech::init()
{
	m_configuration->deprecatedApi()->addVariable("Notify", "NewChat_Speech", true);
}

void Speech::say(const QString &s, const QString &path,
		bool klatt, bool melody,
		const QString &sound_system, const QString &device,
		int freq, int tempo, int basefreq)
{
	kdebugf();

	QString t, dev, soundSystem;
	QStringList list;

	if (path.isEmpty())
	{
		t = m_configuration->deprecatedApi()->readEntry("Speech","SpeechProgram", "powiedz");
		klatt = m_configuration->deprecatedApi()->readBoolEntry("Speech", "KlattSynt");
		melody = m_configuration->deprecatedApi()->readBoolEntry("Speech", "Melody");
		soundSystem = m_configuration->deprecatedApi()->readBoolEntry("Speech", "SoundSystem");
		dev = m_configuration->deprecatedApi()->readEntry("Speech", "DspDev", "/dev/dsp");
		freq = m_configuration->deprecatedApi()->readNumEntry("Speech", "Frequency");
		tempo = m_configuration->deprecatedApi()->readNumEntry("Speech", "Tempo");
		basefreq = m_configuration->deprecatedApi()->readNumEntry("Speech", "BaseFrequency");
	}
	else
	{
		t = path;
		dev = device;
		soundSystem = sound_system;
	}

	if (klatt && soundSystem == "Dsp")
		list.append(" -L");
	if (!melody)
		list.append("-n");
	if (soundSystem == "aRts")
		list.append("-k");
// TODO: dlaczego tak?
//	if (esd)
//		t.append(" -");
	if (soundSystem == "Dsp")
	{
		list.append("-a");
		list.append(dev);
	}
	list.append("-r");
	list.append(QString::number(freq));
	list.append("-t");
	list.append(QString::number(tempo));
	list.append("-f");
	list.append(QString::number(basefreq));

	kdebugm(KDEBUG_INFO, "text: %s command: %s %s\n", qPrintable(s), qPrintable(t), qPrintable(list.join(" ")));

	QProcess *p = new QProcess();
	QProcess::connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), p, SLOT(deleteLater()));
	p->start(t, list);
	p->write(s.toUtf8().constData());
	p->closeWriteChannel();

	kdebugf2();
}

void Speech::notify(const Notification &notification)
{
	kdebugf();

	if (lastSpeech.elapsed() < 1500)
	{
		kdebugf2();
		return;
	}

	QString text;
	QString sex = "Male";

	auto chat = notification.data()["chat"].value<Chat>();

	// TODO:
	if (chat)
	{
		if (isFemale((*chat.contacts().begin()).ownerBuddy().firstName()))
			sex = "Female";
	}

	QString syntax = m_configuration->deprecatedApi()->readEntry("Speech", notification.type() + "_Syntax/" + sex, QString());
	if (syntax.isEmpty())
		text = notification.text();
	else
	{
		QString details = notification.details().join(QStringLiteral("\n"));
		if (details.length() > m_configuration->deprecatedApi()->readNumEntry("Speech", "MaxLength"))
			syntax = m_configuration->deprecatedApi()->readEntry("Speech", "MsgTooLong" + sex);

		syntax = syntax.arg(details);

		if (chat)
		{
			Contact contact = *chat.contacts().begin();
			text = m_parser->parse(syntax, Talkable(contact), &notification, ParserEscape::HtmlEscape);
		}
		else
			text= m_parser->parse(syntax, &notification, ParserEscape::HtmlEscape);
	}

	QTextDocument document;
	document.setHtml(text);
	say(document.toPlainText());
	lastSpeech.restart();

	kdebugf2();
}

NotifierConfigurationWidget * Speech::createConfigurationWidget(QWidget *parent)
{
	return new SpeechConfigurationWidget(parent);
}

/** @} */

#include "moc_speech.cpp"
