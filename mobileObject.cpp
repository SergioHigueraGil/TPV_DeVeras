#include "mobileObject.h"

bool MobileObject::update(float delta) {
	posAndDim.move(vel);
	return true;
}
