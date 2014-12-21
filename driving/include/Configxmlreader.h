/*
	configxmlreader.h
	
	Fichier d'en-tête permettant de récupérer la configuration dans un fichier 
	de format Xml.

	Les pages de configuration doivent être initialisée dan l'objet avant que 
	la lecture de fasse. 
*/

#ifndef __CONFIGXMLREADER_H__
#define __CONFIGXMLREADER_H__

#include <QXmlStreamReader>
#include <QIcon>

class QTreeWidget;
class QTreeWidgetItem;
class QIODevice;
class ConfigWifibotPage;
class ConfigKeyboardPage;

class ConfigXmlReader : public QXmlStreamReader
{
public:
	
	// Constructeur. On donne le widget de façon à pouvoir afficher les données.
	ConfigXmlReader(void);
	// Lecture du fichier de configuration.
	bool read(QIODevice *device);

	// Setteur pour initialiser les pages.
	void setWifibotPage(ConfigWifibotPage *configWifibotPage);
	void setKeyboardPage(ConfigKeyboardPage *configKeyboardPage);

	// Récupération des icons.
	QIcon getWifibotIcon(void) const;
	QIcon getCommandIcon(void) const;
	QIcon getWebcamIcon(void) const;

private:

	// Pages de configuration.
	ConfigWifibotPage *configWifibotPage;
	ConfigKeyboardPage *configKeyboardPage;
	
	// Icon que l'on utilise pour l'affichage.
	QIcon wifibotIcon;
	QIcon commandIcon;
	QIcon webcamIcon;

	// Element inconnu dans le fichier de configuration.
	void readUnknownElement();
	// Lecture du fichier.
	void readXmlConfig(void);
	// Lecture de tous les attributs. (une méthode par attribut lu).
	void readConfigWifiBot(void);
	void readAsservissement(void);
	void readWifiBotList(void);
	void readWifibot(QTreeWidgetItem *item);
	void readName(QTreeWidgetItem *item);
	void readCommand(QTreeWidgetItem *item);
	void readWebcam(QTreeWidgetItem *item);
	void readConfigKeyboard(void);
	void readMoveForwardCommand(void);
	void readMoveBackwardCommand(void);
	void readGoRightCommand(void);
	void readGoLeftCommand(void);
	void readSpeedUpCommand(void);
	void readSpeedDownCommand(void);

	// Création d'un noeud dans l'arbre à afficher.
	QTreeWidgetItem *createChildItem(QTreeWidgetItem *item);
};

#endif
