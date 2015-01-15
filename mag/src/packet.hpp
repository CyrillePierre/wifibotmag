#ifndef PACKET_HPP
#define PACKET_HPP

#include <Arduino.h>
#include "compass.hpp"

/**
 * Cette classe correspond à la trame d'envoi de données sur le réseau.
 * Elle est composé d'un identifiant, du vecteur magnétomètre brute et de
 * l'angle du vecteur.
 */
struct Packet
{
    static int const SIZE = 7;

private:
    byte _buf[SIZE];

public:
    /** @brief Modification de la trame */
    void set(int8_t, int16_t, int16_t, int16_t, float);

    /** @brief Modification de la trame à partir d'un magnétomètre */
    void set(int8_t, Compass const &);

    inline byte const * data() const { return _buf; }
};

#endif // PACKET_HPP
