/*
    ConfigDialog.h

		Interface permettant de configurer les différentes commandes du robot.
*/

#ifndef __CONFIGDIALOG_H__
#define __CONFIGDIALOG_H__

#include <QDialog>

class QString;
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
class ConfigKeyboardPage;
class ConfigWifibotPage;
class ConfigXmlWriter;
class ConfigXmlReader;
class ConfigModel;

class ConfigDialog : public QDialog
{
  Q_OBJECT

public:

	ConfigDialog(ConfigModel *configModel, QWidget *parent = 0);

private:
	
	// Crée les icons qui permettent de choisir la page.
	void createIcons(void);
	
	// Conteneur pour les icons.
	QListWidget *contentsWidget;
	// Conteneur pour les pages.
	QStackedWidget *pagesWidget;

	// Boutton pour sauver le fichier de configuration.
	QPushButton *saveButton;
	// Boutton pour fermer la fenêtre.
	QPushButton *closeButton;

	// Modèle de gestion de la configuration des media.
	ConfigModel *configModel;

	// Nom du fichier de configuration.
	static const QString fileName;

private slots:
	
	// Permet de changer les pages.
	void changePage(QListWidgetItem *current, QListWidgetItem *previous);
	// Initialisation de la boite de dialogue.
	void InitConfigDialog(void);

};

#endif
