#include "rectangle.h"
#include "vector2D.h"
#include "texture.h"
#include "balloon.h"
#include "game.h"

using namespace std;

namespace {
	constexpr float EXPLOSION_FRAME_DELAY = 0.2f;
	constexpr int BALLOON_FRAME_COUNT = 6;
}

Balloon::Balloon(Rectangle position, Vector2D<> velocity, int balloonColor, Texture* balloonTexture, Game* game)
{
	posAndDim = position;
	vel = velocity;
	color = balloonColor;
	textura = balloonTexture;
	juego = game;
	wasHit = false;
	frame = 0;
	frameTime = 0.0f;
}

Balloon::~Balloon() {

}

// Si hay intersección, inicia la animación de explosión una sola vez.
bool Balloon::Hit(const Rectangle& collision)
{
	if (wasHit || !posAndDim.hasIntersection(collision)) {
		return false;
	}

	wasHit = true;
	frame = 0;
	frameTime = 0.0f;
	return true;
}

bool Balloon::update(float delta)
{
	// Una vez alcanzado, el globo deja de moverse y reproduce la explosión.
	if (wasHit && SDL_GetTicks() >= frameTime) {
		frameTime = SDL_GetTicks() + EXPLOSION_FRAME_DELAY;
		++frame;
		if (frame >= BALLOON_FRAME_COUNT) {
			juego->restarGlobosCargados();
			return false;
		}
		return true; 
	}

	MobileObject::update(delta);

	SDL_FRect posActual = posAndDim.to_sdl();
	if (posActual.y + posActual.h <= 0) {
		juego->restarGlobosCargados();
		return false;
	}
	return true;
}

void Balloon::render()const
{
	if (textura != nullptr)
	{
		SDL_FRect pos = posAndDim.to_sdl();
		(*textura).renderFrame(pos, color, frame);
	}
}


