
// Access to the Arduino Libraries
#include <Arduino.h>
#include <Wire.h>

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"
#include "vector2.hpp"

// Define the pin where the led is connected
#define LED_PIN  13
#define BTN_PIN  7

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 mpu;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

// Le vecteur acceillant la mesure du magnétomètre
Vector2<float> mag;
float asax, asay, asaz;

void setup()
{
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
    Serial.begin(115200);

    Serial.println("Initializing I2C devices...");
    mpu.initialize();

    Serial.println("Testing device connections...");
    Serial.print("MPU9150 connection ");
    Serial.println(mpu.testConnection() ? "successful" : "failed");

    // configure Arduino LED for
    pinMode(LED_PIN, OUTPUT);

    mpu.getMagAdjustment(&asax, &asay, &asaz);
}

void loop()
{
    mpu.getMag(&mx, &my, &mz);
    mag.set(mx - 12, my - 100).normalize();

    // display tab-separated accel/gyro x/y/z values
    Serial.print(mag.x); Serial.print(" ");
    Serial.print(mag.y); Serial.print(" ");
    Serial.println(mz);
    Serial.print("angle = "); Serial.print(mag.angleDegree());
    Serial.println(" deg");
    Serial.println(mag.norm());

//    Serial.println();

    delay(200);
}

int main()
{
    bool blinkState = false;

    init();		// Initialize Arduino Librairies
    setup();	// Initialisation de l'application

    while (1)
    {
        // Clignotement
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);

        loop(); // boucle d'exécution
    }

}
