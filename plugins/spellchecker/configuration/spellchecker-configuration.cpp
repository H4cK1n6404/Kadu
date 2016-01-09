/*
 * %kadu copyright begin%
 * Copyright 2011 Sławomir Stępień (s.stepien@interia.pl)
 * Copyright 2011 Michał Ziąbkowski (mziab@o2.pl)
 * Copyright 2011, 2012 Bartosz Brachaczek (b.brachaczek@gmail.com)
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

#include "spellchecker-configuration.h"

#include "spellchecker.h"

#include "configuration/configuration.h"
#include "configuration/deprecated-configuration-api.h"
#include "core/core.h"

SpellcheckerConfiguration::SpellcheckerConfiguration(QObject *parent) :
		QObject{parent},
		Bold{},
		Italic{},
		Underline{},
		Accents{},
		Case{},
		Suggester{},
		SuggesterWordCount{}
{
	createDefaultConfiguration();
	configurationUpdated();
}

SpellcheckerConfiguration::~SpellcheckerConfiguration()
{
}

void SpellcheckerConfiguration::setSpellChecker(SpellChecker *spellChecker)
{
	m_spellChecker = spellChecker;
}

void SpellcheckerConfiguration::createDefaultConfiguration()
{
	Core::instance()->configuration()->deprecatedApi()->addVariable("ASpell", "Bold", "false");
	Core::instance()->configuration()->deprecatedApi()->addVariable("ASpell", "Italic", "false");
	Core::instance()->configuration()->deprecatedApi()->addVariable("ASpell", "Underline", "true");
	Core::instance()->configuration()->deprecatedApi()->addVariable("ASpell", "Color", "#FF0101");
	Core::instance()->configuration()->deprecatedApi()->addVariable("ASpell", "Checked", Core::instance()->configuration()->deprecatedApi()->readEntry("General", "Language"));
	Core::instance()->configuration()->deprecatedApi()->addVariable("ASpell", "Accents", "false");
	Core::instance()->configuration()->deprecatedApi()->addVariable("ASpell", "Case", "false");
	Core::instance()->configuration()->deprecatedApi()->addVariable("ASpell", "Suggester", "true");
	Core::instance()->configuration()->deprecatedApi()->addVariable("ASpell", "SuggesterWordCount", "10");
}

void SpellcheckerConfiguration::configurationUpdated()
{
	bool bold = Core::instance()->configuration()->deprecatedApi()->readBoolEntry("ASpell", "Bold", false);
	bool italic = Core::instance()->configuration()->deprecatedApi()->readBoolEntry("ASpell", "Italic", false);
	bool underline = Core::instance()->configuration()->deprecatedApi()->readBoolEntry("ASpell", "Underline", false);
	bool accents = Core::instance()->configuration()->deprecatedApi()->readBoolEntry("ASpell", "Accents", false);
	bool caseSensivity = Core::instance()->configuration()->deprecatedApi()->readBoolEntry("ASpell", "Case", false);
	bool suggester = Core::instance()->configuration()->deprecatedApi()->readBoolEntry("ASpell", "Suggester", true);
	QColor colorMark("#FF0101");

	auto color = Core::instance()->configuration()->deprecatedApi()->readColorEntry("ASpell", "Color", &colorMark);
	auto checkedEntry = Core::instance()->configuration()->deprecatedApi()->readEntry("ASpell", "Checked", Core::instance()->configuration()->deprecatedApi()->readEntry("General", "Language"));
	auto checked = checkedEntry == "empty" ? QStringList{} : checkedEntry.split(',', QString::SkipEmptyParts);
	int suggesterWordCount = Core::instance()->configuration()->deprecatedApi()->readNumEntry("ASpell", "SuggesterWordCount");

	if (bold == Bold && italic == Italic && underline == Underline && accents == Accents &&
			caseSensivity == Case && suggester == Suggester && color == Color &&
			checked == Checked && suggesterWordCount == SuggesterWordCount)
		return;

	Bold = bold;
	Italic = italic;
	Underline = underline;
	Accents = accents;
	Case = caseSensivity;
	Suggester = suggester;
	Color = color;
	Checked = checked;
	SuggesterWordCount = suggesterWordCount;

	if (m_spellChecker)
	{
		m_spellChecker->buildMarkTag();
		m_spellChecker->buildCheckers();
	}
}

void SpellcheckerConfiguration::setChecked(const QStringList &checked)
{
	if (checked.empty())
		Core::instance()->configuration()->deprecatedApi()->writeEntry("ASpell", "Checked", "empty");
	else
		Core::instance()->configuration()->deprecatedApi()->writeEntry("ASpell", "Checked", checked.join(","));
}

#include "moc_spellchecker-configuration.cpp"
