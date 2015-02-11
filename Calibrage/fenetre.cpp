#include "fenetre.h"

#include "connexion_ui.h"
#include "calibrage_ui.h"
#include "courbe.h"
#include "log.h"
#include "mysocket.h"

#include <QGroupBox>

Fenetre::Fenetre()
{
    log         = new Log();
    con         = new Connexion_ui(log);
    calibrage   = new Calibrage_ui(log);
    curve       = new Courbe(log);

    central     = new QWidget();

    initGrid();
    central->setLayout(grid);

    setCentralWidget(central);

    calibrage->setVisible(false);

    connect(con->getConnection(), SIGNAL(clicked()), this, SLOT(connect_graphic()));
}

void Fenetre::connect_graphic()
{
    // Connexion pour changer la courbe en même temps que les données sont reçus
    connect(con->getSocket(), SIGNAL(changed(QVector2D)), curve, SLOT(setCurve(QVector2D)));

    // Quand on clique sur le bouton commencer, on commence l'acquisition (inversement pour stopper)
    //connect(calibrage, SIGNAL(toggle()), con->getSocket(), SLOT(set_started()));

    // Quand on est co, on active le calibrage
    calibrage->setVisible(true);
}

void Fenetre::initGrid()
{
    grid = new QGridLayout();

    grid->addWidget(con, 0, 0);
    grid->addWidget(calibrage, 1, 0);
    grid->addWidget(log, 2, 0);
    grid->addWidget(curve, 0, 1, 3, 1);

    grid->setColumnStretch(0, 4);
    grid->setColumnStretch(1, 7);
}
