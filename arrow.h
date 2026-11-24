#pragma once
#include "mobileObject.h"

constexpr float BOW_OFFSET_X = 20.f;
constexpr float BOW_OFFSET_Y = 24.f;

class Arrow : public MobileObject {
private:
	int globosExplotados = 0;
public:
	//Crea Arrow
	Arrow(Rectangle position, Vector2D<> velocity, Texture* arrowTexture, Game* game);
	~Arrow();

	void render() const override;
	bool update(float dt) override;

	void Disparar(float arcoX, float arcoY);

	Rectangle getBounds();
};
