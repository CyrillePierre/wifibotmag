
#include <unistd.h>
#include <SDL/SDL.h>
#include <iostream>

#include "gamepad.hpp"
#include "driver.hpp"

int main()
{
	Gamepad	gp;
	Driver  driver;

	int sens = 1;

	gp.addListener(Gamepad::AXIS, [&] () {
//		int left  = ((-gp.leftPedal() + 32768) >> 10) * sens;
		int right = ((-gp.rightPedal() + 32768) >> 10) * sens;
		int dir   = (gp.wheel() >> 9) * sens;

		driver.leftSpeed(right + dir);
		driver.rightSpeed(right - dir);
		driver.sendCmd();

//		std::cout << (int)driver.leftSpeed() << "  " 
//				  << (int)driver.rightSpeed() << "  "
//				  << dir << std::endl;
	});

	gp.addListener(Gamepad::BUTTON, [&sens] () {
		sens *= -1;
	});

	gp.listen();

	return 0;
}
