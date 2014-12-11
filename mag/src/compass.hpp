#ifndef COMPASS_HPP
#define COMPASS_HPP

#include "MPU6050.h"
#include "vector2.hpp"
#include "kalman.hpp"

/**
 * Cette classe représente la boussole (le magnétomètre). Elle ne contient que
 * des méthodes très simples permettant de récupérer l'angle du capteur ainsi
 * que de modifier l'angle de référence.
 */
class Compass
{
public:
    typedef Vector2<float> Vect;

private:
    MPU6050 _mpu;	// Composant (adresse I2C 0x68)
    float	 _ref;	// angle de référence
    Vect	 _vec;	// Vecteur utilisé pour des calculs temporaires
    int		 _mx, _my, _mz;
    Kalman<float> _filter;

public:
    Compass();

    /** @brief Initialisation du capteur */
    void init();

    /** @brief L'angle du magnétomètre (en degré) */
    float getAngleDegree();

    /** @brief L'angle (en radian) */
    float getAngle();

    /** @brief L'angle de référence */
    float getAngleRef();

    /** @brief Modification de la référence */
    void resetRef();

private:
    /** @brief Récupération d'une nouvelle valeur du capteur */
    void update();
};

#endif // COMPASS_HPP






