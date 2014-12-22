/*
	configxmlwriter.cpp
*/

#include "Configxmlwriter.h"
#include "Configwifibotpage.h"
#include "Configkeyboardpage.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QIODevice>
#include <QCheckBox>
#include <QLabel>
#include <QComboBox>

ConfigXmlWriter::ConfigXmlWriter()
{
	setAutoFormatting(true);
}

void ConfigXmlWriter::setWifibotPage(ConfigWifibotPage *configWifibotPage)
{
	Q_ASSERT(configWifibotPage != 0);
	this->configWifibotPage = configWifibotPage;
}

void ConfigXmlWriter::setKeyboardPage(ConfigKeyboardPage *configKeyboardPage)
{
	Q_ASSERT(configKeyboardPage != 0);
	this->configKeyboardPage = configKeyboardPage;
}

/*
 * Ecriture dans le fichier de configuration.
 */
bool ConfigXmlWriter::writeFile(QIODevice *device)
{
	// Device sur lequel on va écrire les données.
	setDevice(device);
	
	// Ecriture d'un document Xml de version 1.0.
	writeStartDocument();
	// Définition du type de document.
	writeDTD("<!DOCTYPE xbel>");
	// Element de départ définissant le langage utilisé.
	writeStartElement("xbel");
	// Spécification de la version du langage xbel.
	writeAttribute("version", "1.0");
	
	// On enregistre la page du Wifibot.
	this->writeWifibotConfig();
	// On enregistre la page du Clavier.
	this->writeKeyboardConfig();
  
	// Fermeture de la balise d'ouverture du document.
	writeEndDocument();
	return true;
}

/*
 * Ecriture de la configuration du Wifibot.
 */
void ConfigXmlWriter::writeWifibotConfig()
{
	// On enregistre la configuration des Wifibots.
	writeStartElement("WifiBotConfiguration");

	// Enregistrement de l'asservissement.
	bool checked = (configWifibotPage->getAsservCheckBox()->checkState() == Qt::Checked);
	writeTextElement("asservissement", checked ? "yes" : "no");
	
	// Enregistrement de la liste.
	writeStartElement("WifiBotList");
	for (int i = 0; i < configWifibotPage->getTreeWidget()->topLevelItemCount(); i++)
		 writeItem(configWifibotPage->getTreeWidget()->topLevelItem(i));
	writeEndElement();

	writeEndElement();
}

/*
 * Ecriture de chaque élément de l'arbre du wifibot.
 */
void ConfigXmlWriter::writeItem(QTreeWidgetItem *item)
{
	// Récupération du nom de la balise.
	QString tagName = item->data(0, Qt::UserRole).toString();

	// Suivant le nom l'enregistrement est différent.
	if (tagName == "WifiBot") 
	{
		// L'affichage est il étendu ou pas?
		bool expanded = configWifibotPage->getTreeWidget()->isItemExpanded(item);
		// Ecriture de la balise.
		writeStartElement(tagName);
		// Définition de l'attribut.
		writeAttribute("expanded", expanded ? "yes" : "no");
		// Nom du Wifibot.
		writeTextElement("name", item->text(0));
		// Ecriture des autres éléments.
		for (int i = 0; i < item->childCount(); ++i)
			writeItem(item->child(i));
		writeEndElement();
	}
	else if (tagName == "command") 
	{
		// Adresse pour commander le robot.
		writeTextElement("command", item->text(0));
	}
	else if (tagName == "webcam") 
	{
		// Adresse pour se connecter à la webcam.
		writeTextElement("webcam", item->text(0));
	}
} 

/*
 * Ecriture de la configuration du Clavier.
 */
void ConfigXmlWriter::writeKeyboardConfig()
{
	// On enregistre la configuration du clavier.
	writeStartElement("KeyboardConfiguration");

	// Ecriture des commandes les unes à la suite des autres.
	writeTextElement("MoveForwardCommand", configKeyboardPage->getMoveForwardCommand()->text());
	writeTextElement("MoveBackwardCommand", configKeyboardPage->getMoveBackwardCommand()->text());
	writeTextElement("GoRightCommand", configKeyboardPage->getGoRightCommand()->text());
	writeTextElement("GoLeftCommand", configKeyboardPage->getGoLeftCommand()->text());
	writeTextElement("SpeedUpCommand", configKeyboardPage->getSpeedUpCommand()->text());
	writeTextElement("SpeedDownCommand", configKeyboardPage->getSpeedDownCommand()->text());

	writeEndElement();
}
