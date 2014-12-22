/*
	configxmlreader.cpp
*/

#include "Configxmlreader.h"
#include "Configwifibotpage.h"
#include "Configkeyboardpage.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QIODevice>
#include <QPixmap>
#include <QCheckBox>
#include <QLabel>
#include <QComboBox>
#include <QFile>
#include <QMessageBox>

ConfigXmlReader::ConfigXmlReader()
{
	// Définition des icon.
	wifibotIcon.addPixmap(QPixmap(":/images/wifibot_vide.png"));
	commandIcon.addPixmap(QPixmap(":/images/controller.png"));
	webcamIcon.addPixmap(QPixmap(":/images/webcam.png"));
}

void ConfigXmlReader::setWifibotPage(ConfigWifibotPage *configWifibotPage)
{
	Q_ASSERT(configWifibotPage != 0);
	this->configWifibotPage = configWifibotPage;
}

void ConfigXmlReader::setKeyboardPage(ConfigKeyboardPage *configKeyboardPage)
{
	Q_ASSERT(configKeyboardPage != 0);
	this->configKeyboardPage = configKeyboardPage;
}


bool ConfigXmlReader::read(QIODevice *device)
{
	// Netoyage de la widget.
	configWifibotPage->getTreeWidget()->clear();

	// Initialisation du fichier dans lequel on va lire.
	setDevice(device);
	
	// Tant que l'on est pas à la fin du fichier.
	while (!atEnd()) 
	{
		// Lire l'élément suivant.
		readNext();
	
		// Si l'élément lu est le premier.
		if (isStartElement()) 
		{
			// Vérification de la version.
			if (name() == "xbel" && attributes().value("version") == "1.0")
				readXmlConfig();
			else
				raiseError(QObject::tr("Le fichier n'est pas un fichier XBEL 1.0."));
		}
	}
	// Retour de l'erreur.
	return !error();
}

void ConfigXmlReader::readUnknownElement()
{
	Q_ASSERT(isStartElement());

	while (!atEnd())
	{
		readNext();

		if (isEndElement())
			break;

		if (isStartElement())
			readUnknownElement();
	}
}

/*
 * Lecture d'une balise principale.
 */
void ConfigXmlReader::readXmlConfig()
{
	// Si le premier élément lu est la balise "xbel".
	Q_ASSERT(isStartElement() && name() == "xbel");

	while (!atEnd()) 
	{
		// Lecture de la balise suivante.
		readNext();
		
		// Si on est à la fin on termine la lecture.
		if (isEndElement())
			 break;
		
		// Si on est sur un balise de début.
		if (isStartElement())
		{
			if (name() == "WifiBotConfiguration")
				readConfigWifiBot();
			else if (name() == "KeyboardConfiguration")
				readConfigKeyboard();
			else
				readUnknownElement();
		}
	}
}

void ConfigXmlReader::readConfigWifiBot()
{
	// Si le premier élément lu est la balise "WifiBotConfiguration".
	Q_ASSERT(isStartElement() && name() == "WifiBotConfiguration");

	while (!atEnd()) 
	{
		// Lecture de la balise suivante.
		readNext();
		
		// Si on est à la fin on termine la lecture.
		if (isEndElement())
			 break;
		
		// Si on est sur un balise de début.
		if (isStartElement())
		{
			if (name() == "asservissement")
				readAsservissement();
			else if (name() == "WifiBotList")
				readWifiBotList();
			else
				readUnknownElement();
		}
	}
}

void ConfigXmlReader::readAsservissement()
{
	// Si le premier élément lu est la balise "WifiBotConfiguration".
	Q_ASSERT(isStartElement() && name() == "asservissement");

	// Récupération de la valeur.
	bool asserv = (readElementText() != "no");
	// Mise à jour de l'affichage au niveau de la page.
	if (asserv)
		configWifibotPage->getAsservCheckBox()->setCheckState(Qt::Checked);
	else
		configWifibotPage->getAsservCheckBox()->setCheckState(Qt::Unchecked);
}

void ConfigXmlReader::readWifiBotList()
{
	// Si le premier élément lu est la balise "WifiBotConfiguration".
	Q_ASSERT(isStartElement() && name() == "WifiBotList");

	while (!atEnd()) 
	{
		// Lecture de la balise suivante.
		readNext();
		
		// Si on est à la fin on termine la lecture.
		if (isEndElement())
			 break;
		
		// Si on est sur un balise de début.
		if (isStartElement())
		{
			if (name() == "WifiBot")
				readWifibot(0);
			else
				readUnknownElement();
		}
	}
}

/*
 * Lecture de la balise "Wifibot".
 */
void ConfigXmlReader::readWifibot(QTreeWidgetItem *item)
{
	// Si la balise n'est pas une balise "WifiBot".
	Q_ASSERT(isStartElement() && name() == "WifiBot");
	
	// Création d'un élément de l'arbre.
	QTreeWidgetItem *wifibot = createChildItem(item);
	bool expanded = (attributes().value("expanded") != "no");
	configWifibotPage->getTreeWidget()->setItemExpanded(wifibot, expanded);
	wifibot->setFlags(wifibot->flags() | Qt::ItemIsEditable);
	wifibot->setIcon(0, wifibotIcon);
	
	while (!atEnd()) 
	{
		// Lecture de la balise suivante.
		readNext();
		
		// A la fin du fichier on arête la lecture.
		if (isEndElement())
			 break;

		// Lecture des trois balises présentes dans la balise "WifiBot".
		if (isStartElement()) 
		{
			if (name() == "name")
				 readName(wifibot);
			else if (name() == "command")
				 readCommand(wifibot);
			else if (name() == "webcam")
				 readWebcam(wifibot);
			else
				 readUnknownElement();
		}
	}
}

void ConfigXmlReader::readName(QTreeWidgetItem *item)
{
	Q_ASSERT(isStartElement() && name() == "name");

	QString name = readElementText();
	item->setText(0, name);
}

void ConfigXmlReader::readCommand(QTreeWidgetItem *item)
{
	Q_ASSERT(isStartElement() && name() == "command");
	
	QTreeWidgetItem *command = createChildItem(item);
	command->setFlags(command->flags() | Qt::ItemIsEditable);
	command->setIcon(0, commandIcon);

	QString textCommand = readElementText();
	if (textCommand == "")
		command->setText(0, QObject::tr("Adresse inconnue"));
	else
		command->setText(0, textCommand);
}

void ConfigXmlReader::readWebcam(QTreeWidgetItem *item)
{
	Q_ASSERT(isStartElement() && name() == "webcam");

	QTreeWidgetItem *webcam = createChildItem(item);
	webcam->setFlags(webcam->flags() | Qt::ItemIsEditable);
	webcam->setIcon(0, webcamIcon);

	QString textWebcam = readElementText();
	if (textWebcam == "")
		webcam->setText(0, QObject::tr("Adresse inconnue"));
	else
		webcam->setText(0, textWebcam);
}

QTreeWidgetItem *ConfigXmlReader::createChildItem(QTreeWidgetItem *item)
{
	QTreeWidgetItem *childItem;
	if (item) 
	{
		childItem = new QTreeWidgetItem(item);
	} 
	else 
	{
		childItem = new QTreeWidgetItem(configWifibotPage->getTreeWidget());
	}
	childItem->setData(0, Qt::UserRole, name().toString());
	return childItem;
} 

QIcon ConfigXmlReader::getWifibotIcon(void) const
{
	return wifibotIcon;
}

QIcon ConfigXmlReader::getCommandIcon(void) const
{
	return commandIcon;
}

QIcon ConfigXmlReader::getWebcamIcon(void) const
{
	return webcamIcon;
}

void ConfigXmlReader::readConfigKeyboard()
{
	// Si le premier élément lu est la balise "WifiBotConfiguration".
	Q_ASSERT(isStartElement() && name() == "KeyboardConfiguration");

	while (!atEnd()) 
	{
		// Lecture de la balise suivante.
		readNext();
		
		// Si on est à la fin on termine la lecture.
		if (isEndElement())
			 break;
		
		// Si on est sur un balise de début.
		if (isStartElement())
		{
			if (name() == "MoveForwardCommand")
				readMoveForwardCommand();
			else if (name() == "MoveBackwardCommand")
				readMoveBackwardCommand();
			else if (name() == "GoRightCommand")
				readGoRightCommand();
			else if (name() == "GoLeftCommand")
				readGoLeftCommand();
			else if (name() == "SpeedUpCommand")
				readSpeedUpCommand();
			else if (name() == "SpeedDownCommand")
				readSpeedDownCommand();
			else
				readUnknownElement();
		}
	}
}

void ConfigXmlReader::readMoveForwardCommand()
{
	Q_ASSERT(isStartElement() && name() == "MoveForwardCommand");
	
	// Initialisation de la commande.
	QString command = readElementText();
	configKeyboardPage->getMoveForwardCommand()->setText(command);
}

void ConfigXmlReader::readMoveBackwardCommand()
{
	Q_ASSERT(isStartElement() && name() == "MoveBackwardCommand");
	
	// Initialisation de la commande.
	QString command = readElementText();
	configKeyboardPage->getMoveBackwardCommand()->setText(command);
}

void ConfigXmlReader::readGoRightCommand()
{
	Q_ASSERT(isStartElement() && name() == "GoRightCommand");
	
	// Initialisation de la commande.
	QString command = readElementText();
	configKeyboardPage->getGoRightCommand()->setText(command);
}

void ConfigXmlReader::readGoLeftCommand()
{
	Q_ASSERT(isStartElement() && name() == "GoLeftCommand");
	
	// Initialisation de la commande.
	QString command = readElementText();
	configKeyboardPage->getGoLeftCommand()->setText(command);
}

void ConfigXmlReader::readSpeedUpCommand()
{
	Q_ASSERT(isStartElement() && name() == "SpeedUpCommand");
	
	// Initialisation de la commande.
	QString command = readElementText();
	configKeyboardPage->getSpeedUpCommand()->setText(command);
}

void ConfigXmlReader::readSpeedDownCommand()
{
	Q_ASSERT(isStartElement() && name() == "SpeedDownCommand");
	
	// Initialisation de la commande.
	QString command = readElementText();
	configKeyboardPage->getSpeedDownCommand()->setText(command);
}

