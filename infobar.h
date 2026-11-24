#pragma once

#include <SDL3/SDL.h>
#include "gameObject.h"

class Texture;

class InfoBar : public GameObject {
private:
    SDL_FRect Recta;
    SDL_Color Fondo;
    Texture* Digitos;
    int flechas;
    int puntos;

public:
    //Crea lla infobar con sus parametros
    InfoBar(float x, float y, float ancho, float alto, Texture* digitsTex, Game* game);
    ~InfoBar();

    //Renderiza la infobar
    void render() const override;
    bool update(float dt) override;
};
