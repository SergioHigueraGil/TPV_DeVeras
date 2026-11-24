#include "arrow.h"
#include "game.h"

using namespace std;

Arrow::Arrow(Rectangle position, Vector2D<> velocity, Texture* arrowTexture, Game* game) {
	this->posAndDim = position;
	this->vel = velocity;
	this->textura = arrowTexture;
	this->juego = game;
}

Arrow::~Arrow() {

}

bool Arrow::update(float dt) {
	MobileObject::update(dt);

	SDL_FRect posActual = posAndDim.to_sdl(); 

	
	//Detecta si sale de la pantalla
	if (posActual.x > Game::WINDOW_WIDTH) {
		juego->restarFlechasCargadas();
		return false;
	}
	

	// La punta es el cuadrado del tamaño de la altura de la flecha, situado en su extremo derecho.
	Rectangle punta(
		Point2D<>{posActual.x + posActual.w - posActual.h, posActual.y},
		posActual.h,
		posActual.h);

	// La flecha sigue avanzando aunque alcance uno o varios globos.
	if (juego != nullptr) {
		if(juego->checkCollision(punta)) {
			juego->sumarPuntosGlobo(globosExplotados);
			globosExplotados++;
		}
	}

	// Comentario de prueba

	return true;
}

void Arrow::render() const {
	if (textura != nullptr)
	{
		//Define el render de arrow
		SDL_FRect pos = posAndDim.to_sdl();
		(*textura).renderFrame(pos, 0, 0);
	}
}

//Dispara la flecha del arco
void Arrow::Disparar(float arcoX, float arcoY)
{
	posAndDim = Rectangle(Point2D<>{arcoX + BOW_OFFSET_X, arcoY + BOW_OFFSET_Y}, 64.f, 16.f);
}

Rectangle Arrow::getBounds() {
	return posAndDim;
}
