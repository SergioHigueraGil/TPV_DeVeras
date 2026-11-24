#pragma once
#include "gameError.h"


class FileNotFound : public GameError {
public:
	FileNotFound(string a) : GameError("Error 404: Archivo" + a + "no encontrado") {};
};
