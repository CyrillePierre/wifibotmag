/*
    AbstractCommand.h

	Fichier de base dont toutes les sortes de commandes doit dériver.
*/

#ifndef __ABSTRACTCOMMAND_H__
#define __ABSTRACTCOMMAND_H__

#include <QObject>

class Controler;
class ConfigModel;

class AbstractCommand : public QObject
{
public:

	AbstractCommand(ConfigModel *configModel, Controler *controler, QObject *parent = 0);
	virtual ~AbstractCommand();

	// Lancement de la commande.
	virtual void start(void) = 0;
	// Arrêt de la commande.
	virtual void stop(void) = 0;

protected:
	
	// Toute commande est reliée au controleur du Wifibot.
	Controler *controler;
	// Toute commande possède le modèle de configuration de façon à pouvoir accéder à sa configuration.
	ConfigModel *configModel;

public slots:

	// Toute modification du fichier de configuration entraine une mise à jour de la configuration de la commande.
	virtual void onSavedConfig(void) = 0;

	// Méthode d'initialisation de l'asservissement du robot.
	void setControl(void);

};

#endif
