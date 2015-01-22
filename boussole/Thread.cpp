#include "Thread.h"

#include <iostream>

/**
 * @brief : Méthode principale du thread
 */
void Thread::run()
{
    while(1)
    {
        // On récupère la trame
        std::cin.read(buffer, SIZE);

        // On la formate comme on veut
        formatValue();

        emit changed2(value);
    }
}

void Thread::formatValue()
{
    short v1  = buffer[SIZE - 2] & 0x0f << 8; // On recupère les 4 premiers bits de l'angle
    char v2  = buffer[SIZE - 1];             // On récupère les 8 derniers

    value = v1 | v2;                         // On fusionne les 2

    // On la met sous le bon format [-180, 180] -> [0, 360]
    value = 360 - value;
}


