#pragma once
#include "sceneObject.h"
#include "vector2D.h"

class MobileObject : public SceneObject{
protected:
	Vector2D<> vel;
public:
	bool update(float dt) override;
};
