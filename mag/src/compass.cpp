
#include <Wire.h>
#include "compass.hpp"

/* Ces constantes servent à corriger l'erreur statique du capteur.
 * Apparement il y a un biais (peut être dû à un champ magnétique) */
const int XOFFSET = 12;	// Décalage en X de la mesure
const int YOFFSET = 100;	// Décalage en Y de la mesure

Compass::Compass() : _ref(0), _filter(100, 20)
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
float Compass::getAngleDegree()
{
    update();
    return _filter.get(_vec.rotate(-_ref).angleDegree());
}

/**
 * @return l'angle par rapport à la référence (en radian)
 */
float Compass::getAngle()
{
    update();
    return _vec.rotate(-_ref).angle();
}

/**
 * @return l'angle de référence
 */
float Compass::getAngleRef()
{
    update();
    return _ref;
}

/**
 * Cette méthode permet de définir l'angle actuel du capteur comme étant
 * le nouvel angle de référence.
 */
void Compass::resetRef()
{
    update();
    _ref = _vec.angle();
}

/**
 * Pour mettre à jour la valeur lue, il suffit d'appeler la méthode du capteur
 * permettant de récupérer le vecteur champ magnétique.
 */
void Compass::update()
{
    _mpu.getMag(&_mx, &_my, &_mz);
    _vec.set(_mx - XOFFSET, _my - YOFFSET);
}
