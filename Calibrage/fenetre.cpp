#include "fenetre.h"

#include "connexionUI.h"
#include "calibrageUI.h"
#include "courbe.h"
#include "log.h"
#include "mysocket.h"

#include <QGroupBox>

Fenetre::Fenetre()
{
    con         = new ConnexionUI();
    calibrage   = new CalibrageUI();
    curve       = new Courbe();

    central     = new QWidget();

    initGrid();
    central->setLayout(grid);

    setCentralWidget(central);

    calibrage->setVisible(false);

    connect(con->getConnection(), SIGNAL(clicked()), this, SLOT(connectGraphic()));
}

void Fenetre::connectGraphic()
{
    // Connexion pour changer la courbe en même temps que les données sont reçus
    connect(con->getSocket(), SIGNAL(changed(QPointF)), curve, SLOT(setCurve(QPointF)));

    // Quand on clique sur le bouton commencer, on commence l'acquisition (inversement pour stopper)
    connect(calibrage, SIGNAL(toggle(bool)), con->getSocket(), SLOT(setStarted(bool)));

    // Quand on clique sur le bouton calibrage, on recalibre les données
    connect(calibrage->getCalibrate(), SIGNAL(clicked()), curve, SLOT(calibrate()));

    // Quand on clique sur commencer, on supprime tous les points
    connect(calibrage, SIGNAL(startAcquisition()), curve, SLOT(clear()));

    connect(calibrage->getSend(), SIGNAL(clicked()), con->getSocket(), SLOT(writeBiais()));

    // Quand on est co, on active le calibrage
    calibrage->setVisible(true);
}

void Fenetre::initGrid()
{
    grid = new QGridLayout();

    grid->addWidget(con, 0, 0);
    grid->addWidget(calibrage, 1, 0);
    grid->addWidget(Log::get(), 2, 0);
    grid->addWidget(curve, 0, 1, 3, 1);

    grid->setColumnStretch(0, 4);
    grid->setColumnStretch(1, 7);
}
