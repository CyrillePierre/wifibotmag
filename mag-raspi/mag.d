
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <math.h>

#define GYRO_SENSI	131.0f
#define ACCEL_SENSI	16384.0f

#define ACCEL_ID  0x68

#define SERVO_X   7
#define SERVO_Y   6

#define PULSE_MAX 220
#define PULSE_MIN 50
#define HALF_RANGE (PULSE_MAX - PULSE_MIN) / 2

/**
 * Cette fonction permet de lire une valeur de l'accéléromètre. La valeur
 * est codé en complément à 2 mais la fonction convertie la valeur dans un
 * format plus lisible.
 */
int readAccelValue(int fd, int addr)
{
	int high = wiringPiI2CReadReg8(fd, addr),
		low  = wiringPiI2CReadReg8(fd, addr + 1),
		val  = (high << 8) + low;

	return (val & 0x8000) ? -((0xffff - val) + 1) : val;
}

/**
 * Cette fonction permet de lire les valeurs des 6 capteurs du MPU-6050 et
 * de calculer la valeur absolue des différents angles.
 */
void calcRotation(int fd)
{
	static unsigned char first = 1;
	static float igx, igy;

	if (first) {
		igx = readAccelValue(fd, 0x43) / GYRO_SENSI;
		igy = readAccelValue(fd, 0x45) / GYRO_SENSI;
		first = 0;
	}

	// Lecture des valeurs du capteur
	float gx = readAccelValue(fd, 0x43) / GYRO_SENSI,
		  gy = readAccelValue(fd, 0x45) / GYRO_SENSI,
		  gz = readAccelValue(fd, 0x47) / GYRO_SENSI,
		  ax = readAccelValue(fd, 0x3b) / ACCEL_SENSI,
		  ay = readAccelValue(fd, 0x3d) / ACCEL_SENSI,
		  az = readAccelValue(fd, 0x3f) / ACCEL_SENSI;

	// calcul des quaternions
//	MadgwickAHRSupdateIMU(gx - igx, gy - igy, gz, ax, ay, az);

//	printf("q0 = %6f, q1 = %6f, q2 = %6f, q3 = %6f\n", q0, q1, q2, q3);
}


float dist(float a, float b)
{
	return sqrt(a * a + b * b);
}

void calc(int fd)
{
	static unsigned char first = 1;
	static float igx, igy;

	if (first) {
		igx = readAccelValue(fd, 0x43) / GYRO_SENSI;
		igy = readAccelValue(fd, 0x45) / GYRO_SENSI;
		first = 0;
	}

	// Lecture des valeurs du capteur
	float gx = readAccelValue(fd, 0x43) / GYRO_SENSI,
		  gy = readAccelValue(fd, 0x45) / GYRO_SENSI,
		  gz = readAccelValue(fd, 0x47) / GYRO_SENSI,
		  ax = readAccelValue(fd, 0x3b) / ACCEL_SENSI,
		  ay = readAccelValue(fd, 0x3d) / ACCEL_SENSI,
		  az = readAccelValue(fd, 0x3f) / ACCEL_SENSI,
		  mx,
		  my,
		  mz;

	float aax = atan2(ay, dist(ax, az)),
		  aay = atan2(ax, dist(ay, az));

	printf("gyro  : [%f,%f,%f]\n", gx, gy, gz);
	printf("accel : [%f,%f,%f]\n", ax, ay, az);
	printf("mag   : [%f,%f,%f]\n", mx, my, mz);
}

int main()
{
	int accel_fd, mag_fd;

	// Chargement de l'accéléromètre
	if ((accel_fd = wiringPiI2CSetup(ACCEL_ID)) < 0) {
		perror("wiringPiI2CSetup() accel");
		return 1;
	}

	// Initialisation de l'accéléromètre
//	wiringPiI2CWriteReg8(accel_fd, 0x6b, 0);

	// Activation du magnétomètre
	wiringPiI2CWriteReg8(accel_fd, 0x37, 0x02);
	delay(10);

	if ((mag_fd = wiringPiI2CSetup(0x0c)) < 0) {
		perror("wiringPiI2CSetup() accel");
		return 1;
	}

//	wiringPiI2CWriteReg8(mag_fd, 0x0a, 0x01);
//	delay(10);
	printf("%x\n", wiringPiI2CReadReg8(mag_fd, 0x00));
	
	for (;;) {
//		calcRotation(accel_fd);
//		calc(accel_fd);

		wiringPiI2CWriteReg8(mag_fd, 0x0a, 0x01);
		delay(1);
		printf("%d\n", wiringPiI2CReadReg8(mag_fd, 0x03));

		delay(100);
	}

	return 0;
}
