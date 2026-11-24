#pragma once
#include "rectangle.h"
#include "gameObject.h"
#include "texture.h"

class SceneObject : public GameObject{
protected:
	Rectangle posAndDim;
	Texture* textura;
public:
	virtual bool Hit(const Rectangle&);
	virtual ~SceneObject() = default;
};
