#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>
#include <list>
#include <istream>
#include <vector>
#include <random>
#include "gameObject.h"
#include "sceneObject.h"
#include "mobileObject.h"
#include "bow.h"
#include "arrow.h"
#include "balloon.h"
#include "rectangle.h"
#include "infobar.h"
#include "butterfly.h"
#include "gameError.h"

// Declaraciones anticipadas
class Texture;

/**
 * Clase principal del juego.
 */
class Game
{
public:
	// Número de actualizaciones por segundo
	static constexpr int FRAME_RATE = 30;
	// Periodo entre actualizaciones en milisegundos
	static constexpr int FRAME_PERIOD = 1000 / FRAME_RATE;
	// Tamaño real de la ventana
	static constexpr int WINDOW_WIDTH = 800;
	static constexpr int WINDOW_HEIGHT = 600;

	enum TextureName
	{
		BACKGROUND = 0,
		BOW, 
		ARROW,
		BALLOON,
		DIGITS,
		BUTTERFLY,
		NUM_TEXTURES
		
	};

private:

	// Lista de todos los objetos cargados
	std::list<SceneObject*> objetosCargados;

	//Array de guardado de texturas
	std::array<Texture*, NUM_TEXTURES> textures;

	//Puntuación
	int puntuacionActual = 0;

	//Cuentas de objetos cargados
	int numFlechasCargadas = 0;
	int numGlobosCargados = 0;

	int numMariposasIniciales = 0;

	// Elemenetos de juego (se eliminarán en el futuro)
	Bow* bow;
	InfoBar* infoBar;

	//Datos que extraer de mi archivo
	int nivelActual = 1;
	void clearLevel();
	bool tryLoadNextLevel();
	SDL_Color colorFondoJuego; 
	int flechasIniciales;     
	int balDelay;
	int balpoints;           
	int goal;
	bool exit;
	
	// Variable de seguimiento de tiempo
	float time;

	// Elementos aleatorios
	std::random_device rd;
	float rando;

	// Dimensiones de la ventana
	SDL_Window* window;
	SDL_Renderer* renderer;

	void render() const;
	void update(float delta);
	void handleEvents();

	//Carga el archivo
	bool loadLevel(const std::string& archivo);
	// Carga archivo de guardadoç
	void loadSaveFile(const std::string& archivo);
	// Guarda en archivo de guardado
	void saveGame();


public:

	Game();
	~Game();

	// Obtiene una textura por su nombre
	Texture* getTexture(TextureName name) const;

	int getPuntuacion() const;
	int getFlechas() const;
	SDL_Renderer* getRenderer() const;

	// Ejecuta el bucle principal del juego
	void run();

	// Comprueba si hay algún objeto colocado en ese rectángulo
	bool checkCollision(const Rectangle& rect);

	// Sumador de puntos por globo
	void sumarPuntosGlobo(int globosPrevios = 0);;

	//Sumador de flechas cargadas
	void sumarFlechasCargadas();

	// Restador de flechas cargadas
	void restarFlechasCargadas();

	void restarGlobosCargados();

	//Restador de puntos por mariposa
	void restarPuntosMariposa(int puntos);

	//Generador de numeros aleatorios
	float rando_rng(float menor, float mayor);

	Texture* getTexture(int);

	void pushArrow(Arrow*);

};



#endif // GAME_H
