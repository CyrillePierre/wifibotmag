/*
	configxmlwriter.h

	Fichier d'en-tête permettant l'enregistrement de la configuration dans un fichier 
	de format Xml.
*/

#ifndef __CONFIGXMLWRITER_H__
#define __CONFIGXMLWRITER_H__

#include <QXmlStreamWriter>

class QTreeWidget;
class QTreeWidgetItem;
class QIODevice;
class ConfigWifibotPage;
class ConfigKeyboardPage;

class ConfigXmlWriter : public QXmlStreamWriter
{
public:

	// Constructeur. On donne le TreeWidget pour pouvoir faire l'affichage.
	ConfigXmlWriter(void);
	// Permet d'écrire la configuration dans un fichier.
	bool writeFile(QIODevice *device);

	// Setteur pour initialiser les pages.
	void setWifibotPage(ConfigWifibotPage *configWifibotPage);
	void setKeyboardPage(ConfigKeyboardPage *configKeyboardPage);

private:

	// Pages de configuration.
	ConfigWifibotPage *configWifibotPage;
	ConfigKeyboardPage *configKeyboardPage;

	// Widget dans lequel on stocke l'arbre de configuration des wifibots.
	QTreeWidget *treeWidget;
	
	// Ecriture de la configuration du Wifibot.
	void writeWifibotConfig(void);
	// Permet d'écrire les configurations élément par élément pour l'arbre des Wifibots.
	void writeItem(QTreeWidgetItem *item);
	// Ecriture de la configuration du clavier.
	void writeKeyboardConfig(void);
   
};

#endif
