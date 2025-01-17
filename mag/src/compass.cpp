
#include <Wire.h>
#include "compass.hpp"

Compass::Compass() : _ref(0), _bx(0), _by(0), _bz(0), _filter(.5, 10, 0)
{
}

/**
 * Initialisation du capteur
 */
void Compass::init()
{
    Wire.begin();
    _mpu.initialize();
}

/**
 * @return l'angle par rapport à la référence (en degrée)
 */
float Compass::getAngleDegree() const
{
    return _vec.angleDegree();
}

/**
 * @return l'angle par rapport à la référence (en radian)
 */
float Compass::getAngle() const
{
    return _vec.angle();
}

/**
 * @return l'angle de référence
 */
float Compass::getAngleRef() const
{
    return _ref;
}

/**
 * Cette méthode permet de définir l'angle actuel du capteur comme étant
 * le nouvel angle de référence.
 */
void Compass::resetRef()
{
    _ref = _vec.angle();
}

/**
 * Cette méthode permet de palier le défaut du capteur où les conditions
 * environnementales qui faussent les valeurs des capteurs.
 * @param bx biais observé sur le capteur de l'axe x
 * @param by biais observé sur le capteur de l'axe y
 * @param bz biais observé sur le capteur de l'axe z
 */
void Compass::calibrate(int16_t bx, int16_t by, int16_t bz)
{
    _bx += bx;
    _by += by;
    _bz += bz;
}

/**
 * Pour mettre à jour la valeur lue, il suffit d'appeler la méthode du capteur
 * permettant de récupérer le vecteur champ magnétique.
 */
void Compass::update()
{
    _mpu.getMag(&_mx, &_my, &_mz);
    _vec.set(mx(), my()).rotate(-_ref);
}
