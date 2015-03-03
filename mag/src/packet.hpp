#ifndef PACKET_HPP
#define PACKET_HPP

#include <Arduino.h>
#include "compass.hpp"

/**
 * Cette classe correspond à la trame d'envoi de données sur le réseau.
 * Elle est composée d'un identifiant, du vecteur magnétomètre brute et de
 * l'angle du vecteur.
 */
struct Packet
{
    static int const		SIZE = 9;

    // Identifiant possible
    static int8_t const	DATA  = 0;
    static int8_t const	RESET = 1;
    static int8_t const	CALIB = 2;

private:
    byte _buf[SIZE];

public:
    /** @brief Modification de la trame */
    void set(int8_t, int16_t, int16_t, int16_t, float);

    /** @brief Modification de la trame à partir d'un magnétomètre */
    void set(int8_t, Compass const &);

    inline byte const * data() const { return _buf; }
    inline byte * data() { return _buf; }
    inline int8_t id() const { return _buf[0]; }

    inline int16_t x() const {
        return _buf[2] + (((int16_t) _buf[1]) << 8);
    }

    inline int16_t y() const {
        return _buf[4] + (((int16_t) _buf[3]) << 8);
    }

    inline int16_t z() const {
        return _buf[6] + (((int16_t) _buf[5]) << 8);
    }

    inline int16_t a() const {
        return _buf[8] + (((int16_t) _buf[7]) << 8);
    }
};

#endif // PACKET_HPP






