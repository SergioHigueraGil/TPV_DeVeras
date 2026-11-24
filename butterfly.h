#pragma once
#include <vector>
#include "mobileObject.h"

//Tamaño de frame de butterfly
constexpr int TAM_FRAME_MARIPOSA_X = 32; 
constexpr int TAM_FRAME_MARIPOSA_Y = 32;

constexpr int PENALIZACION_MARIPOSA = 10;

class Butterfly : public MobileObject {
private:
	//Atributos de butterfly
	int color;
	bool wasHit;
	int frame;
	float frameTime;
	float animTimer;    // Temporizador en segundos para animación
	float dirTimer;     // Temporizador en segundos para dirección

	void randomDir();

public:
	Butterfly(Rectangle position, Vector2D<> velocity, int butterflyColor, Texture* butterflyTexture, Game* game);
	~Butterfly();
	bool Hit(const Rectangle&) override;
	void render() const override;
	bool update(float delta) override;
};
#pragma once
