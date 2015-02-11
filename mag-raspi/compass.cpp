
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "compass.hpp"

Compass::Compass()
	: _mag_fd(-1),
	  _ref(0),
	  _vec(0, 0),
	  _mx(0), _my(0), _mz(0),
	  _bx(0), _by(0), _bz(0),
	  _filter(.5, 10, 0)
{
}

/**
 * Initialisation du capteur
 */
void Compass::init()
{
	int mpu_fd;

	// Config d'un fd pour communiquer avec le MPU6050
	if ((mpu_fd = wiringPiI2CSetup(0x68)) < 0) {
		perror("wiringPiI2CSetup() accel");
	}

	// Initialisation de l'accéléromètre
	wiringPiI2CWriteReg8(mpu_fd, 0x6b, 0);

	// Activation du magnétomètre
	wiringPiI2CWriteReg8(mpu_fd, 0x37, 0x02);

	if ((_mag_fd = wiringPiI2CSetup(0x0c)) < 0) {
		perror("wiringPiI2CSetup() accel");
	}

	close(mpu_fd);
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
	// Pour lire le capteur, il faut d'abord activer le mode single measurement
	wiringPiI2CWriteReg8(_mag_fd, 0x0a, 0x01);

	// Un délai de 10 millisecondes est nécéssaire pour que les capteurs
	// aient le temps de faire une mesure
	delay(10);

	// Lecture des valeurs des capteurs
	_mx = wiringPiI2CReadReg16(_mag_fd, 0x03);
	_my = wiringPiI2CReadReg16(_mag_fd, 0x05);
	_mz = wiringPiI2CReadReg16(_mag_fd, 0x07);

    _vec.set(mx(), my()).rotate(-_ref);
}

