
#include <iostream>
#include <thread>
#include "gamepad.hpp"

Gamepad::Gamepad()
{
	// Initialisation de la SDL pour la gestion des joysticks
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
		std::cerr << "Erreur: SDL_init(): " << SDL_GetError() << std::endl;
		exit(1);
	}

	if (!SDL_NumJoysticks()) {
		std::cerr << "Erreur: Pas de joysticks détectés" << std::endl;
		exit(1);
	}

	// On ouvre la première joystick en espérant que le volant est la seule
	// manette connectée à l'ordi.
	if (!(_js = SDL_JoystickOpen(0))) {
		std::cerr << "Erreur: SDL_JoystickOpen(); " << SDL_GetError()
				  << std::endl;
		exit(1);
	}
}

Gamepad::~Gamepad()
{
	SDL_JoystickClose(_js);
	SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
}

void Gamepad::listen()
{
	SDL_Event	event;

	while (_listening && SDL_WaitEvent(&event) ) {
		switch (event.type) {
			case SDL_JOYAXISMOTION: notifyAll(AXIS);		break;
			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP:	notifyAll(BUTTON);		break;
			case SDL_QUIT:			_listening = false;		break;
			default:				notifyAll(OTHER);		break;
		}
	}
}

/**
 * Cette méthode permet de lancer un thread qui s'occupera d'écouter tous les
 * évènements généré par le gamepad. Si un évènement intervient, le thread
 * préviendra les listeners qui écoute l'évènement concerné.
 */
void Gamepad::asyncListen()
{
	std::thread([this] () { listen(); }).detach();
}

/**
 * Cette méthode va interrompre le thread démarré par la méthode asyncListen()
 */
void Gamepad::stopListen()
{
	_listening = false;
}

/**
 * Cette méthode permet de prévenir tous les abonnées qu'un évènement
 * s'est produit.
 */
void Gamepad::notifyAll(Event evt) const
{
	auto range = _listeners.equal_range(evt);
	auto listener = range.first;
	while (listener != range.second)
		((listener++)->second)();

}
