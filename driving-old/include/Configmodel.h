/*
	configmodel.h
	
	Fichier contenant le modèle du fichier de configuration.
	Ainsi, tous les éléments de l'application peuvent accéder aux données
	via cet interface.

	Il suffit de se connecter au signal envoyé à chaque modification
	et de récupérer les éléments souhaités.
*/

#ifndef __CONFIGMODEL_H__
#define __CONFIGMODEL_H__

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMultiMap>

class ConfigXmlWriter;
class ConfigXmlReader;
class ConfigWifibotPage;
class ConfigKeyboardPage;

class ConfigModel : public QObject
{
	Q_OBJECT

private:

	// Nom du fichier de configuration.
	static const QString fileName;

	// Second modèle de la liste des WifiBots.
	QMultiMap<QString, QString> *mapWifibot; 

	// Parser permettant de lire ou écrire le fichier de configuration.
	ConfigXmlWriter *parserXmlWriter;
	ConfigXmlReader *parserXmlReader;

	// Page permettant d'afficher la configuration.
	ConfigWifibotPage *configWifibotPage;
	ConfigKeyboardPage *configKeyboardPage;

public:

	// Constructeur / destructeur.
	ConfigModel(QObject *parent = 0);
	~ConfigModel();

	// Accesseurs pour récupérer les pages.
	ConfigWifibotPage *getWifibotPage(void) const;
	ConfigKeyboardPage *getKeyboardPage(void) const;

	// Configuration des Wifibots.
	bool isWifibotControled(void) const;
	QStringList getWifibotList(void) const;
	QString getCommandAddress(const QString &wifibot) const;
	QString getWebcamAddress(const QString &wifibot) const;

	// Configuration du Clavier.
	QString getMoveForwardKey(void) const;
	QString getMoveBackwardKey(void) const;
	QString getGoRightKey(void) const;
	QString getGoLeftKey(void) const;
	QString getSpeedUpKey(void) const;
	QString getSpeedDownKey(void) const;
   

signals:

	// Signal envoyé à chaque enregistrement du fichier de configuration.
	void update(void);
	// Lors d'un enregistrement, on informe l'utilisateur.
	void updateStatusBar(const QString &message);

public slots:

	// Chargement du fichier de configuration.
	void open(void);
	// Enregistrement du fichier de configuration.
	void save(void);
	// Ajout d'un élément Wifibot dans l'arbre.
	void onAddWifibot(void);
	// Suppression d'un Wifibot.
	void onDelWifibot(void);
	
};

#endif

