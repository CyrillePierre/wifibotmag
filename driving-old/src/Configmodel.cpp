/*
	configmodel.cpp
*/

#include "Configmodel.h"
#include "Configxmlwriter.h"
#include "Configxmlreader.h"
#include "Configwifibotpage.h"
#include "Configkeyboardpage.h"

#include <QTreeWidget>
#include <QVariant>
#include <QFile>
#include <QMessageBox>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>

const QString ConfigModel::fileName = "configuration.bot";

ConfigModel::ConfigModel(QObject *parent)
    : QObject(parent)
{
	mapWifibot = new QMultiMap<QString, QString>;

	// Création des pages.
	configWifibotPage = new ConfigWifibotPage;
	configKeyboardPage = new ConfigKeyboardPage;
   
	// Création des parsers.
	parserXmlWriter = new ConfigXmlWriter;
	parserXmlWriter->setKeyboardPage(configKeyboardPage);
	parserXmlWriter->setWifibotPage(configWifibotPage);
 	
   
   parserXmlReader = new ConfigXmlReader;
	parserXmlReader->setKeyboardPage(configKeyboardPage);
	parserXmlReader->setWifibotPage(configWifibotPage);

	// Ajout d'un Wifibot dans l'arbre.
	connect(configWifibotPage, SIGNAL(addWifibot()), this, SLOT(onAddWifibot()));
	// Suppression d'un Wifibot.
	connect(configWifibotPage, SIGNAL(delWifibot()), this, SLOT(onDelWifibot()));
}

ConfigModel::~ConfigModel()
{
	if (parserXmlWriter)
		delete parserXmlWriter;
	if (parserXmlReader)
		delete parserXmlReader;
}

void ConfigModel::open()
{ 
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)) 
	{
		QMessageBox::warning(reinterpret_cast<QWidget *>(this->parent()), 
												tr("Interactive-Bot"), tr("Impossible de lire le fichier %1:\n%2.")
												.arg(fileName).arg(file.errorString()));
		return;
	}
	
	// Lecture du fichier de configuration.
	if (!parserXmlReader->read(&file)) 
	{
		 QMessageBox::warning(reinterpret_cast<QWidget *>(this->parent()), tr("Interactive-Bot"),
													tr("Erreur de lecture du fichier %1 à la ligne %2, colonne %3:\n%4")
													.arg(fileName)
													.arg(parserXmlReader->lineNumber())
													.arg(parserXmlReader->columnNumber())
													.arg(parserXmlReader->errorString()));
	}
	else
		emit update();
}

void ConfigModel::save()
{
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text)) 
	{
		QMessageBox::warning(reinterpret_cast<QWidget *>(this->parent()), 
												tr("Interactive-Bot"), tr("Impossible d'enregistrer le fichier %1:\n%2.")
												.arg(fileName).arg(file.errorString()));
		return;
	}

	// Enregistrement du fichier.
	if (parserXmlWriter->writeFile(&file))
	{
		// Emmission du signal pour informer l'utilisateur.
		emit update();
		emit updateStatusBar(tr("Etat : Fichier de configuration sauvé"));
	}
}

bool ConfigModel::isWifibotControled() const
{
	return configWifibotPage->getAsservCheckBox()->isChecked();
}

QStringList ConfigModel::getWifibotList() const
{
	// Netoyage de la map.
	mapWifibot->clear();
	// On récupère l'arbre des WifiBots.
	QTreeWidget *treeWidget = configWifibotPage->getTreeWidget();

	// Remplissage de la map.
	for (int i = 0; i < treeWidget->topLevelItemCount(); i++)
	{
		// Récupération du nom du Wifibot.
		QString currentWifibot = treeWidget->topLevelItem(i)->data(0, Qt::DisplayRole).toString();
		// Récupération des adresses.
		mapWifibot->insert(currentWifibot, treeWidget->topLevelItem(i)->child(0)->data(0, Qt::DisplayRole).toString());
		mapWifibot->insert(currentWifibot, treeWidget->topLevelItem(i)->child(1)->data(0, Qt::DisplayRole).toString());
	}
	return QStringList(mapWifibot->uniqueKeys());
}

void ConfigModel::onAddWifibot()
{
	QTreeWidgetItem *wifibotItem = new QTreeWidgetItem;
	wifibotItem->setIcon(0, parserXmlReader->getWifibotIcon());
	wifibotItem->setText(0, tr("Nouveau Wifibot"));
	wifibotItem->setData(0, Qt::UserRole, "WifiBot");
	wifibotItem->setFlags(wifibotItem->flags() | Qt::ItemIsEditable);

	QTreeWidgetItem *commandItem = new QTreeWidgetItem(wifibotItem);
	commandItem->setIcon(0, parserXmlReader->getCommandIcon());
	commandItem->setText(0, tr("Nouvelle adresse de commande"));
	commandItem->setData(0, Qt::UserRole, "command");
	commandItem->setFlags(commandItem->flags() | Qt::ItemIsEditable);

	QTreeWidgetItem *webcamItem = new QTreeWidgetItem(wifibotItem);
	webcamItem->setIcon(0, parserXmlReader->getWebcamIcon());
	webcamItem->setText(0, tr("Nouvelle adresse de webcam"));
	webcamItem->setData(0, Qt::UserRole, "webcam");
	webcamItem->setFlags(webcamItem->flags() | Qt::ItemIsEditable);

	// Ajout du nouveau Wifibot.
	configWifibotPage->getTreeWidget()->addTopLevelItem(wifibotItem);
	wifibotItem->setExpanded(true);
}

void ConfigModel::onDelWifibot()
{
	// On supprime l'élément sélectionné.
	delete configWifibotPage->getTreeWidget()->currentItem();
}

QString ConfigModel::getCommandAddress(const QString &wifibot) const
{
	return mapWifibot->values(wifibot).at(1);
}

QString ConfigModel::getWebcamAddress(const QString &wifibot) const
{
	return mapWifibot->values(wifibot).at(0);
}


QString ConfigModel::getMoveForwardKey() const
{
	return configKeyboardPage->getMoveForwardCommand()->text();
}

QString ConfigModel::getMoveBackwardKey() const
{
	return configKeyboardPage->getMoveBackwardCommand()->text();
}

QString ConfigModel::getGoRightKey() const
{
	return configKeyboardPage->getGoRightCommand()->text();
}

QString ConfigModel::getGoLeftKey() const
{
	return configKeyboardPage->getGoLeftCommand()->text();
}

QString ConfigModel::getSpeedUpKey() const
{
	return configKeyboardPage->getSpeedUpCommand()->text();
}

QString ConfigModel::getSpeedDownKey() const
{
	return configKeyboardPage->getSpeedDownCommand()->text();
}

ConfigWifibotPage *ConfigModel::getWifibotPage() const
{
	return configWifibotPage;
}

ConfigKeyboardPage *ConfigModel::getKeyboardPage() const
{
	return configKeyboardPage;
}


