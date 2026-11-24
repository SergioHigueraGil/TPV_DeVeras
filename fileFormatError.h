#pragma once
#include "gameError.h"


class FileFormatError : public GameError {
public:
	FileFormatError(string a, int b) : GameError("Error de formato en el archivo " + a + " línea " + to_string(b)) {};
};
