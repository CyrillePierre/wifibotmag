#ifndef FENETRE_H
#define FENETRE_H

#include <QMainWindow>
#include <QGridLayout>

class ConnexionUI;
class CalibrageUI;
class Courbe;
class Log;

/**
 * @brief Fenetre de l'application
 */
class Fenetre : public QMainWindow
{
    Q_OBJECT

public:
    Fenetre();

public slots:
    void connectGraphic();

private:
    void initGrid();

    QWidget *central;
    QGridLayout *grid;

    ConnexionUI *con;
    CalibrageUI *calibrage;
    Courbe       *curve;
    Log          *log;
};

#endif // FENETRE_H
