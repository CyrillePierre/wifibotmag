
#pragma once

#include <functional>
#include <utility>
#include <map>
#include <SDL2/SDL.h>
#include "stdint.h"

/**
 * Cette classe fait office de driver pour le volant. Elle est utilisé comme
 * une surcouche à la SDL pour écouter tous les évènements du volant et
 * des pédales. Utilisation du design patern observateur pour gérer le
 * changement d'état du périphérique.
 * @author Cyrille PIERRE
 */
struct Gamepad
{
	enum Event {
		AXIS,		// un changement s'est produit sur l'un des axes
		BUTTON,		// un changement d'état d'un bouton s'est produit
		OTHER		// un autre évènement s'est produit
	};

	typedef std::multimap<Event, std::function<void()>> Listeners;

private:
	bool			_listening;
	SDL_Joystick *	_js;
	Listeners		_listeners;

public:
	/** @brief Constructeur */
	Gamepad();

	/** @brief Destructeur */
	~Gamepad();

	/** @brief Ajout d'un listener pour un évènement */
	template<typename Function>
	void addListener(Event, Function &&);

	/** @brief Écoute des évènement du gamepad */
	void listen();

	/** @brief Écoute asynchrone des évènements du gamepad */
	void asyncListen();

	/** @brief Interruption de l'écoute du gamepad */
	void stopListen();

	// Getteurs
	inline int16_t wheel()	    const { return SDL_JoystickGetAxis(_js, 0); }
	inline int16_t leftPedal()  const { return SDL_JoystickGetAxis(_js, 1); }
	inline int16_t rightPedal() const { return SDL_JoystickGetAxis(_js, 2); }

private:
	/** @brief Notification des listeners */
	void notifyAll(Event) const;
};


/**
 * Cette méthode permet d'ajouter un listener à la liste des listeners
 * disponibles pour un évènement donné. Il est donc possible de mettre plusieurs
 * listeners pour un même évènement.
 * @param evt : l'évènement auquel le listener doit être attaché
 * @param fn : le listener (une fonction de type void())
 */
template<typename Function>
void Gamepad::addListener(Event evt, Function && fn)
{
	_listeners.insert(std::make_pair(evt, std::forward<Function>(fn)));
}
