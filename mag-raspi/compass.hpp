#ifndef COMPASS_HPP
#define COMPASS_HPP

#include <stdint.h>
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
	int		_mag_fd;		// file descriptor de l'I2C du magnétomètre
    float	_ref;			// angle de référence
    Vect	_vec;			// Vecteur utilisé pour des calculs temporaires
    int16_t _mx, _my, _mz;	// valeurs des capteurs
    int16_t	_bx, _by, _bz;	// Biais des capteurs

    Kalman<float> _filter;

public:
    Compass();

    /** @brief Initialisation du capteur */
    void init();

    /** @brief L'angle du magnétomètre (en degré) */
    float getAngleDegree() const;

    /** @brief L'angle (en radian) */
    float getAngle() const;

    /** @brief L'angle de référence */
    float getAngleRef() const;

    /** @brief Modification de la référence */
    void resetRef();

    /** @brief Calibration des capteurs */
    void calibrate(int16_t bx, int16_t by, int16_t bz);

    // Valeurs brutes (avec le biais de corrigé)
    inline int16_t mx() const { return _mx - _bx; }
    inline int16_t my() const { return _my - _by; }
    inline int16_t mz() const { return _mz - _bz; }

    /** @brief Récupération d'une nouvelle valeur du capteur */
    void update();
};

#endif // COMPASS_HPP






