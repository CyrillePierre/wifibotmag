#include "Thread.h"

#include <iostream>
#include <unistd.h>
#include <stdint.h>

/**
 * @brief : Méthode principale du thread
 */
void Thread::run()
{
	int a;
    while(1)
    {
        // On la formate comme on veut
        if ((a = read(0, buffer, 8)) == 8) {
			formatValue();
			emit changed(value);
//			std::cout << value << std::endl;
		}
    }
}

void Thread::formatValue()
{
    int16_t v1  = buffer[7] << 8; // On recupère les 4 premiers bits de l'angle
    v1  |= buffer[8];             // On récupère les 8 derniers

    value = ((double) v1) / (1 << 7);

    // On la met sous le bon format [-180, 180] -> [0, 360]
    //value = 180 - value;

    if(value < 0)
        value += 360;
}


