
// Access to the Arduino Libraries
//#include <Arduino.h>
//#include <Ethernet.h>

#include <iostream>
#include <functional>
#include <mutex>
#include <chrono>
#include <thread>
#include <unistd.h>

#include "compass.hpp"
#include "packet.hpp"
#include "server.hpp"


// Période entre 2 envois de données (en millisecondes)
std::chrono::milliseconds const DATA_PERIOD(20);

// Port du serveur
int const PORT(1337);

std::mutex compass_mutex;

/**
 * Cette fonction représente la gestion d'un client. Lorsqu'un client se
 * connecte, on ne fait rien d'autre que d'écouter en boucle ce qu'il a à
 * dire. En fonction de la commande qu'il enverra, on effectura la tâche
 * aproprié.
 * @param c    : le client concerné
 * @param comp : le magnétomètre
 */
void clientTask(Client const & c, Compass & comp)
{
	Packet pkt;

	// On écoute en boucle ce que le client veut nous dire
	while (c.read(pkt.data(), Packet::SIZE) > 0) {
		switch (pkt.id()) {
		case Packet::RESET: 	// Commande reset
			compass_mutex.lock();
			comp.resetRef();
			compass_mutex.unlock();
			break;

		case Packet::CALIB:	// Commande de calibrage
			compass_mutex.lock();
			comp.calibrate(pkt.x(), pkt.y(), pkt.z());
			compass_mutex.unlock();
			break;
		}
	}

	c.close();
}

int main()
{
	std::chrono::time_point<std::chrono::system_clock> nextTime;
	Packet  pkt;
	Compass comp;
	Server  server(PORT);

	comp.init();		// Initialisation du magnétomètre.
	server.connect();	// Connexion du serveur

	// Gestion des clients : la fonction clientTask sera exécuté par
	// chaque thread gérant un client.
	using namespace std::placeholders;
	server.asyncAcceptLoop(std::bind(clientTask, _1, std::ref(comp)));

	// Calcul de l'heure + la période du prochain envoi
	nextTime = std::chrono::system_clock::now() + DATA_PERIOD;

	// Boucle d'envoi des données
	while (1) {
		// MAJ du capteur (thread safe)
		compass_mutex.lock();
		comp.update();
		compass_mutex.unlock();

		pkt.set(Packet::DATA, comp);

		// On envoi les nouvelles valeurs à tous ceux qui sont connectés
		server.writeAll(pkt.data(), Packet::SIZE);

//		std::cout << '[' << comp.mx() << ',' << comp.my()
//				  << ',' << comp.mz() << "] : "
//				  << comp.getAngleDegree() << std::endl;

		// On endort le thread de manière à ce qu'il y ait toujours
		// DATA_PERIOD millisecondes entre 2 envois de données
		std::this_thread::sleep_until(nextTime);
		nextTime += DATA_PERIOD;
	}

	server.disconnect();

	return 0;
}
