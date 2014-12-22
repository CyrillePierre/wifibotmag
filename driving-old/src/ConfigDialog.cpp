/*
	ConfigDialog.cpp
*/

#include <QtGui>

#include "ConfigDialog.h"
#include "Configwifibotpage.h"
#include "Configkeyboardpage.h"
#include "Configxmlwriter.h"
#include "Configxmlreader.h"
#include "Configmodel.h"

const QString ConfigDialog::fileName = "configuration.xbel";

ConfigDialog::ConfigDialog(ConfigModel *configModel, QWidget *parent)
	: QDialog(parent), configModel(configModel)
{
	this->setWindowTitle(tr("Configuration"));
	this->setWindowModality(Qt::WindowModal);

	contentsWidget = new QListWidget(this);
	contentsWidget->setViewMode(QListView::IconMode);
	contentsWidget->setIconSize(QSize(96, 84));
	contentsWidget->setMovement(QListView::Static);
	contentsWidget->setMaximumWidth(128);
	contentsWidget->setSpacing(12);

	// Création des pages de la boite de dialogue.
	pagesWidget = new QStackedWidget;
	pagesWidget->addWidget(configModel->getKeyboardPage());
	pagesWidget->addWidget(configModel->getWifibotPage());
	pagesWidget->addWidget(configModel->getWifibotPage());
	
	saveButton = new QPushButton(tr("Sauver"));
	connect(saveButton, SIGNAL(clicked()), configModel, SLOT(save()));

	closeButton = new QPushButton(tr("Quitter"));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
	
	this->createIcons();
	// La page affichée sera celle correspondant au premier icône.
	contentsWidget->setCurrentRow(0);

	QHBoxLayout *horizontalLayout = new QHBoxLayout;
	horizontalLayout->addWidget(contentsWidget);
	horizontalLayout->addWidget(pagesWidget, 1);

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch(1);
	buttonsLayout->addWidget(saveButton);
	buttonsLayout->addWidget(closeButton);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(horizontalLayout);
	mainLayout->addStretch(1);
	mainLayout->addSpacing(12);
	mainLayout->addLayout(buttonsLayout);
	setLayout(mainLayout);

	// Ouverture et chargement du fichier de configuration.
	configModel->open();
}

void ConfigDialog::createIcons()
{
	QListWidgetItem *clavierButton = new QListWidgetItem(contentsWidget);
	clavierButton->setIcon(QIcon(":/images/clavier.png"));
	clavierButton->setText(tr("Clavier"));
	clavierButton->setTextAlignment(Qt::AlignHCenter);
	clavierButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	QListWidgetItem *wifibotButton = new QListWidgetItem(contentsWidget);
	wifibotButton->setIcon(QIcon(":/images/wifibot.png"));
	wifibotButton->setText(tr("WifiBot"));
	wifibotButton->setTextAlignment(Qt::AlignHCenter);
	wifibotButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	connect(contentsWidget,
				 SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
				 this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
}

void ConfigDialog::InitConfigDialog()
{
	// Ouverture et chargement du fichier de configuration.
	configModel->open();
	// Affichage de la boite de dialogue.
	show();
}

void ConfigDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
	if (!current)
		current = previous;

	pagesWidget->setCurrentIndex(contentsWidget->row(current));
} 