#include "rectangle.h"
#include "vector2D.h"
#include "texture.h"
#include "butterfly.h"
#include "game.h"

using namespace std;

namespace {
	constexpr float ANIM_FRAME_DELAY = 100.0f;      // Velocidad de animacion
	constexpr float DIR_CHANGE_DELAY = 2000.0f;     // Cambio de dirección 
	constexpr int BUTTERFLY_FRAME_COUNT = 10;       
	constexpr float FALL_SPEED = 4.0f;             
}

Butterfly::Butterfly(Rectangle position, Vector2D<> velocity, int butterflyColor, Texture* butterflyTexture, Game* game)
{
	posAndDim = position;
	vel = velocity;
	color = butterflyColor;
	textura = butterflyTexture;
	juego = game;
	wasHit = false;
	frame = 0;
	frameTime = 0.0f;
	dirTimer = SDL_GetTicks() + DIR_CHANGE_DELAY;
}

Butterfly::~Butterfly() {

}

// Si hay intersección, resta puntos y empieza a caer hacia abajo
bool Butterfly::Hit(const Rectangle& collision)
{
	if (wasHit || !posAndDim.hasIntersection(collision)) {
		return false;
	}

	wasHit = true;
	vel = Vector2D<>{ 0.0f, FALL_SPEED };
	juego->restarPuntosMariposa(PENALIZACION_MARIPOSA);
	return true;
}

bool Butterfly::update(float delta)
{
	if (wasHit) {
		MobileObject::update(delta);
		SDL_FRect posActual = posAndDim.to_sdl();
		if (posActual.y >= Game::WINDOW_HEIGHT) {
			return false;
		}
		return true;
	}

	if (SDL_GetTicks() >= frameTime) {
		frameTime = SDL_GetTicks() + ANIM_FRAME_DELAY;
		frame = (frame + 1) % BUTTERFLY_FRAME_COUNT;
	}

	if (SDL_GetTicks() >= dirTimer) {
		dirTimer = SDL_GetTicks() + DIR_CHANGE_DELAY;

		float vx = juego->rando_rng(-2.5f, 2.5f);
		float vy = juego->rando_rng(-2.5f, 2.5f);

		Vector2D<> nuevaVel(vx, vy);
		vel = nuevaVel;
	}

	MobileObject::update(delta);

	SDL_FRect posActual = posAndDim.to_sdl();
	if ((posActual.x < 0.0f && vel.getX() < 0) || (posActual.x + posActual.w > Game::WINDOW_WIDTH && vel.getX() > 0)) {
		vel = Vector2D<>{ -vel.getX(), vel.getY() };
	}
	if ((posActual.y < 0.0f && vel.getY() < 0) || (posActual.y + posActual.h > Game::WINDOW_HEIGHT && vel.getY() > 0)) {
		vel = Vector2D<>{ vel.getX(), -vel.getY() };
	}

	return true;
}

void Butterfly::render()const
{
	if (textura != nullptr)
	{
		SDL_FRect pos = posAndDim.to_sdl();
		(*textura).renderFrame(pos, color, frame);
	}
}


