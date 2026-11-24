//
// Tecnología de la Programación de Videojuegos 1
// Facultad de Informática UCM
//
// Plantilla de proyecto con SDL
//

#include "game.h"

int main(int argc, char* argv[])
{
	//Ejecuta el juego
	try {
		Game().run();
	}
	catch (...) {

	}
	// TODO: manejar excepciones

	return 0;
}
