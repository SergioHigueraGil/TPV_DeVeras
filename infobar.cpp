#include "infobar.h"
#include "texture.h"
#include "game.h"

InfoBar::InfoBar(float x, float y, float ancho, float alto, Texture* digitsTex, Game* game)
    : Digitos(digitsTex), flechas(0), puntos(0)
{
    juego = game;
    Recta = { x, y, ancho, alto };
    Fondo = { 215, 215, 215, 200 };
}

InfoBar::~InfoBar() {

}

bool InfoBar::update(float dt) {
    puntos = juego->getPuntuacion();
    flechas = juego->getFlechas();
    return true;
}

void InfoBar::render() const {
    SDL_Renderer* renderer = juego->getRenderer();
    if (!renderer || !Digitos) return;

    //Crea y colorea el infobar
    SDL_SetRenderDrawColor(renderer, Fondo.r, Fondo.g, Fondo.b, Fondo.a);
    SDL_RenderFillRect(renderer, &Recta);

    float posy = Recta.y + (Recta.h - 30.f) / 2.f;

    //Puntuacion
    float posX_Puntos = Recta.x + 20.f; 

    if (puntos >= 100) {
        // Tres dígitos
        Digitos->renderFrame(SDL_FRect{ posX_Puntos, posy, 30.f, 30.f }, 0, puntos / 100);
        Digitos->renderFrame(SDL_FRect{ posX_Puntos + 30.f, posy, 30.f, 30.f }, 0, (puntos / 10) % 10);
        Digitos->renderFrame(SDL_FRect{ posX_Puntos + 60.f, posy, 30.f, 30.f }, 0, puntos % 10);
    }
    else if (puntos >= 10) {
        // Dos dígitos
        Digitos->renderFrame(SDL_FRect{ posX_Puntos, posy, 30.f, 30.f }, 0, puntos / 10);
        Digitos->renderFrame(SDL_FRect{ posX_Puntos + 30.f, posy, 30.f, 30.f }, 0, puntos % 10);
    }
    else {
        // Un digito
        Digitos->renderFrame(SDL_FRect{ posX_Puntos, posy, 30.f, 30.f }, 0, puntos);
    }
        
    //Flechas
    float posX_Flechas = Recta.x + Recta.w - 80.f;

    if (flechas >= 10) {
        Digitos->renderFrame(SDL_FRect{ posX_Flechas, posy, 30.f, 30.f }, 0, flechas / 10);
    }
    Digitos->renderFrame(SDL_FRect{ posX_Flechas + 30.f, posy, 30.f, 30.f }, 0, flechas % 10);
}

