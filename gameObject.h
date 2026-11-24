#pragma once
class Game;

class GameObject {
protected:
	Game* juego;
public:
	virtual void render() const = 0;
	virtual bool update(float dt) = 0;
};
