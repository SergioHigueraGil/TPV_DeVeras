#pragma once
#include <vector>
#include "mobileObject.h"

//Tamaño de frame de globo
int const TAM_FRAME_GLOBO_X = 148/3;
int const TAM_FRAME_GLOBO_Y = 164/3;
// Número de globos a generar 
int const NUM_GLOBOS_A_GENERAR = 10;
// Numero de colores de los globos
int const NUM_COLORES_GLOBOS = 7;

class Balloon : public MobileObject{
private:
	//Atributos de ballon
	int color;
	bool wasHit;
	int frame;
	float frameTime;
public:
	Balloon(Rectangle position, Vector2D<> velocity, int ballonColor, Texture* ballonTexture, Game* game);
	~Balloon();
	bool Hit(const Rectangle&) override;
	void render() const override;
	bool update(float delta) override;
};
