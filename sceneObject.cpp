#include "sceneObject.h"

bool SceneObject::Hit(const Rectangle& collision) {
	if (!posAndDim.hasIntersection(collision)) return false;
	return true;
}
