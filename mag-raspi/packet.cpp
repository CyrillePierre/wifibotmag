
#include "packet.hpp"

/**
 * @param id l'identifiant
 * @param mx valeur du capteur selon l'axe X
 * @param my valeur du capteur selon l'axe Y
 * @param mz valeur du capteur selon l'axe Z
 * @param ang angle en coordonnée polaire du vecteur
 */
void Packet::set(int8_t id, int16_t mx, int16_t my, int16_t mz, float ang)
{
    int16_t a = (int16_t) ang;

//    _buf[0]  = (id & 0x0f)   << 4;	// id
//    _buf[0] |= (mx & 0x1e00) >> 9;	// mx, 4 plus gros bits
//    _buf[1]  = (mx & 0x01fe) >> 1;	// mx, 8 bits suivant
//    _buf[2]  = (mx & 0x0001) << 7;	// mx, dernier bit
//    _buf[2] |= (my & 0x1fc0) >> 6;	// my, 7 bits de poids fort
//    _buf[3]  = (my & 0x003f) << 2;	// my, 6 derniers bits
//    _buf[3] |= (mz & 0x1800) >> 11;	// mz, 2 bits de poids fort
//    _buf[4]  = (mz & 0x07f8) >> 3;	// mz, 8 bits suivants
//    _buf[5]  = (mz & 0x0007) << 5;	// mz, 3 derniers bits
//    _buf[5] |= (a  & 0x1f00) >> 8;	// angle, 5 bits de poid fort
//    _buf[6]  =  a  & 0x00ff;		// angle, 8 derniers bits

    _buf[0] = id;
    _buf[1] = (mx & 0xff00) >> 8;
    _buf[2] = mx & 0xff;
    _buf[3] = (my & 0xff00) >> 8;
    _buf[4] = my & 0xff;
    _buf[5] = (mz & 0xff00) >> 8;
    _buf[6] = mz & 0xff;
    _buf[7] = (a & 0xff00) >> 8;
    _buf[8] = a & 0xff;
}

/**
 * @param id l'identifiant
 * @param comp le magnétomètre
 */
void Packet::set(int8_t id, Compass const & comp)
{
    set(id, comp.mx(), comp.my(), comp.mz(), comp.getAngleDegree());
}






