#pragma once

#include "rectangle.h"
#include "vector2D.h"
#include "sceneObject.h"

class Bow : public SceneObject{
private:
	Vector2D<> vel;
	int numflechas;
	bool armed;


public:
	Bow(Rectangle position, Vector2D<> velocity, int arrows, bool isArmed, Texture* bowTexture, Game* game);
	~Bow();

	void handleEvent(const SDL_Event& event);
	void render() const override;
	bool update(float delta) override;

	//Devuelve posicion del arco
	float getY() const;
	float getX() const;

	//Recive y gasta las flechas
	int getFlechas() const;
	void gastarFlecha();
	void setFlechas(int nuevasFlechas) { numflechas = nuevasFlechas; armed = true; }

	//Identifica si el arco esta armado
	bool isArmed() const;
	bool isUnArmed();
};
