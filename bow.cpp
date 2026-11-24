#include "rectangle.h"
#include "vector2D.h"
#include "texture.h"
#include "bow.h"
#include "game.h"

SDL_FRect TamPos;

Bow::Bow(Rectangle position, Vector2D<> velocity, int arrows, bool isArmed, Texture* bowTexture, Game* game)
{
    //Atributos del arco
    posAndDim = position;
    vel = velocity;
    numflechas = arrows;
    armed = isArmed;
    textura = bowTexture;
	juego = game; 
}
	
Bow::~Bow()
{
}

void Bow::handleEvent(const SDL_Event& event)
{
    //Controla el movimiento del arco y permite cargarlo
    if (event.type == SDL_EVENT_KEY_DOWN) 
    {
        switch (event.key.key) 
        {
        case SDLK_UP:
            vel = Vector2D<>{ 0.0f, -3.0f };
            break;
        case SDLK_DOWN:
            vel = Vector2D<>{ 0.0f, 3.0f };  
            break;    
        case SDLK_LEFT:
           if (numflechas > 0) armed = false;
            break;

        }
    }
    else if (event.type == SDL_EVENT_KEY_UP) 
    {
        switch (event.key.key) 
        {
        case SDLK_UP:
            if (vel.getY() < 0.0f) vel = Vector2D<>{ 0.0f, 0.0f };
            break;
        case SDLK_DOWN:
            if (vel.getY() > 0.0f) vel = Vector2D<>{ 0.0f, 0.0f };
            break;
        }
    }

    //Funcinamiento de disparo
    if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_RIGHT && event.key.repeat == 0) {
        if (numflechas > 0 && !isArmed()) {

            Point2D<> posInicial(0.f, 0.f);
            Rectangle cajaInicial(posInicial, 64.f, 16.f);
            Vector2D<> velInicial(12.f, 0.f);

            GameObject* nuevaFlecha = new Arrow(cajaInicial, velInicial, juego->getTexture(2), juego);

            Arrow* flecha = dynamic_cast<Arrow*>(nuevaFlecha);

            flecha->Disparar(posAndDim.to_sdl().x, posAndDim.to_sdl().y);

            juego->pushArrow(flecha);

            juego->sumarFlechasCargadas();

            gastarFlecha();

            isUnArmed();
        }
    }

}

bool Bow::update(float delta) 
{
    //Mueve y limita el arco
    posAndDim.move(vel);
    SDL_FRect posActual = posAndDim.to_sdl();

    if (posActual.y < 0.0f) 
    { 
        posAndDim.move(Vector2D<>{0.0f, -posActual.y});
        vel = Vector2D<>{ 0.0f, 0.0f };
    }

    float limiteInferior = Game::WINDOW_HEIGHT - posActual.h; 
    if (posActual.y > limiteInferior) 
    {
        float diferencia = limiteInferior - posActual.y;
        posAndDim.move(Vector2D<>{0.0f, diferencia});
        vel = Vector2D<>{ 0.0f, 0.0f };
    }

    return true;
}

void Bow::render()const {
    //Elige que textura del arco se muestra
    if (textura != nullptr) {
        struct SDL_FRect pos = posAndDim.to_sdl();
       
        int arcoFrame = 0;

        if (armed == true) 
        {
            arcoFrame = 1;
        }

        (*textura).renderFrame(pos, 0, arcoFrame);
    }
}

float Bow::getY() const { return posAndDim.to_sdl().y; }
float Bow::getX() const { return posAndDim.to_sdl().x; }

int Bow::getFlechas() const { return numflechas; }
void Bow::gastarFlecha() { numflechas--; }

bool Bow::isArmed() const { return armed; }
bool Bow::isUnArmed() { armed = true; return armed; }


