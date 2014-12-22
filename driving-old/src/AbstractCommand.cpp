
#include "Controler.h"
#include "Configmodel.h"
#include "AbstractCommand.h"

AbstractCommand::AbstractCommand(ConfigModel *configModel, Controler *controler, QObject *parent)
	: QObject(parent)
{
	this->controler = controler;
	this->configModel = configModel;

	// Lors d'un enregistrement, on gère l'asservissement.
	connect(configModel, SIGNAL(update()), this, SLOT(setControl()));
}

AbstractCommand::~AbstractCommand()
{
}

void AbstractCommand::setControl()
{
	if (configModel->isWifibotControled())
		controler->setControl(true);
	else
		controler->setControl(false);
}
