/*
	configglovepage.cpp
*/

#include <QtGui>
#include <QStringList>

#include "Configwifibotpage.h"

ConfigWifibotPage::ConfigWifibotPage(QWidget *parent)
 : QWidget(parent)
{
	QLabel *titleLabel = new QLabel;
	QFont font;
	font.setPointSize(12);
	font.setBold(true);
	font.setWeight(75);
	titleLabel->setFont(font);
	titleLabel->setText(tr("Configuration des WifiBots"));

	asservCheckBox = new QCheckBox(tr("Mode asservi"), this);
	QGroupBox *asservGroup = new QGroupBox(tr("Asservissement"), this);
	QVBoxLayout *asservLayout = new QVBoxLayout;
	asservLayout->addWidget(asservCheckBox);
	asservGroup->setLayout(asservLayout);

	wifibotIcon.addPixmap(QPixmap(":/images/wifibot_vide.png"));
	wifibotIconAdd.addPixmap(QPixmap(":/images/wifibot_add.png"));
	wifibotIconDel.addPixmap(QPixmap(":/images/wifibot_del.png"));

	QGroupBox *configGroup = new QGroupBox(tr("Liste des WifiBots"), this);

	treeWidget = new QTreeWidget(configGroup);
	treeWidget->setMinimumHeight(250);
	QTreeWidgetItem *header = new QTreeWidgetItem;
	header->setText(0, tr("WifiBots"));
	header->setIcon(0, wifibotIcon);
	treeWidget->setHeaderItem(header);

	addButton = new QPushButton(wifibotIconAdd, tr("Ajouter"), configGroup);
	delButton = new QPushButton(wifibotIconDel, tr("Supprimer"), configGroup);
	delButton->setDisabled(true);

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch(1);
	buttonLayout->addWidget(addButton);
	buttonLayout->addWidget(delButton);

	QVBoxLayout *treeLayout = new QVBoxLayout;
	treeLayout->addWidget(treeWidget);
	treeLayout->addLayout(buttonLayout);
	configGroup->setLayout(treeLayout);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(titleLabel);
	mainLayout->addSpacing(10);
	mainLayout->addWidget(asservGroup);
	mainLayout->addSpacing(5);
	mainLayout->addWidget(configGroup);
	setLayout(mainLayout);

	// Liaison des signaux et slots.
	connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(onItemClicked(QTreeWidgetItem *, int)));
	connect(addButton, SIGNAL(clicked()), this, SIGNAL(addWifibot()));
	connect(delButton, SIGNAL(clicked()), this, SIGNAL(delWifibot()));
	connect(delButton, SIGNAL(clicked()), this, SLOT(onDelButtonClick()));
}

QTreeWidget *ConfigWifibotPage::getTreeWidget() const
{
	return treeWidget;
}

QCheckBox *ConfigWifibotPage::getAsservCheckBox() const
{
	return asservCheckBox;
}

void ConfigWifibotPage::onItemClicked(QTreeWidgetItem *item, int column)
{
	// Si on a cliqué sur un élément Wifibot.
	if (item->data(0, Qt::UserRole).toString() == "WifiBot")
		delButton->setEnabled(true);
	else 
		delButton->setEnabled(false);
}

void ConfigWifibotPage::onDelButtonClick()
{
	delButton->setEnabled(false);
}