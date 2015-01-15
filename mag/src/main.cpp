
// Access to the Arduino Libraries
#include <Arduino.h>
#include <Ethernet.h>

#include "compass.hpp"
#include "packet.hpp"

// Define the pin where the led is connected
#define LED_PIN  13
#define BTN_PIN  7

Compass comp;
Packet   pkt;

// Configuration de l'adresse de la carte
byte       mac[] = { 0xB0, 0x0B, 0x5A, 0xDA, 0xDA, 0xDA };
IPAddress ip(192, 168, 0, 44);
IPAddress dnsServer(192, 168, 0, 42);
IPAddress gateway(192, 168, 0, 42);
IPAddress subnet(255, 255, 255, 0);

// On utilisera le port 1337
EthernetServer server(1337);

void setup()
{
    // Initialisation de la connexion ethernet
    Ethernet.begin(mac, ip, dnsServer, gateway, subnet);
    server.begin();

    // join I2C bus (I2Cdev library doesn't do this automatically)
    Serial.begin(115200);

    comp.init();

    pinMode(LED_PIN, OUTPUT);
    pinMode(BTN_PIN, INPUT_PULLUP);
}

void loop()
{
    comp.update();
    pkt.set(0, comp);

//    Serial.println(pkt.ang / 10000.);

    // Ecriture sur le réseau
    server.write(pkt.data(), Packet::SIZE);

    // Envoi de l'angle sur le réseau
    EthernetClient client = server.available();
    if (client && client.available()) {
        String cmd = client.readStringUntil('\n');

        if (cmd == "reset")
            comp.resetRef();
    }

    if (!digitalRead(BTN_PIN))
        comp.resetRef();

    delay(20);
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
