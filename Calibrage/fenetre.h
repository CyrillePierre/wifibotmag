#ifndef FENETRE_H
#define FENETRE_H

#include <QMainWindow>
#include <QGridLayout>

class Connexion_ui;
class Calibrage_ui;
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
    void connect_graphic();

private:
    void initGrid();

    QWidget *central;
    QGridLayout *grid;

    Connexion_ui *con;
    Calibrage_ui *calibrage;
    Courbe       *curve;
    Log          *log;
};

#endif // FENETRE_H
