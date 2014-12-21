/*
	ConfigWifibotPage.h

	Fichier d'en-tête des pages à afficher dans la boite de 
	dialogue qui permet de configurer les WifiBots.
*/

#ifndef __CONFIGWIFIBOTPAGE_H__
#define __CONFIGWIFIBOTPAGE_H__

#include <QWidget>
#include <QIcon>

class QTreeWidget;
class QTreeWidgetItem;
class QPushButton;
class QCheckBox;

class ConfigWifibotPage : public QWidget
{
	Q_OBJECT

public:

	ConfigWifibotPage(QWidget *parent = 0);
	
	// Accesseur pour le modèle de la configuration.
	QTreeWidget *getTreeWidget(void) const;
	QCheckBox *getAsservCheckBox(void) const;

private:
	
	// Asservissement ou non du Wifibot.
	QCheckBox *asservCheckBox;

	// Permet d'afficher l'arbre de configuration des wifibots.
	QTreeWidget *treeWidget;
	// Icons du WifiBot.
	QIcon wifibotIcon;
	QIcon wifibotIconAdd;
	QIcon wifibotIconDel;

	// Bouttons pour ajouter ou supprimer un Wifibot.
	QPushButton *addButton;
	QPushButton *delButton;

private slots:
	
	// On a sélectionné un element de l'arbre.
	void onItemClicked(QTreeWidgetItem *item, int column);
	// On veut supprimer un Wifibot.
	void onDelButtonClick(void);

signals:

	// On veut ajouter un Wifibot.
	void addWifibot(void);
	// On veut supprimer un Wifibot.
	void delWifibot(void);
};

#endif 