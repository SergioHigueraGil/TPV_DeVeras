#include "game.h"

#include <string>

#include <SDL3_image/SDL_image.h>

#include "texture.h"

#include "bow.h"

#include <list>

#include <random>

#include "infobar.h"

#include <fstream>

#include <stdexcept>

#include "fileFormatError.h"

#include "fileNotFound.h"

#include "sdlError.h"

using namespace std;

// Constantes
constexpr const char* const WINDOW_TITLE = "Bow & Arrow 1.0";
constexpr const char* IMAGE_ROOT = "../assets/images/";
constexpr const char* LEVEL_ROOT = "../assets/levels/";
constexpr const char* SAVEFILE_ROOT = "../assets/";


// Estructura para especificar las texturas que hay que
// cargar y el tamaño de su matriz de frames
struct TextureSpec
{
	const char* name;
	int nrows = 1;
	int ncols = 1;
};

constexpr array<TextureSpec, Game::NUM_TEXTURES> textureList{
	TextureSpec {"background1.png", 1, 1}, {"bow.png", 1, 2},
	{"arrow.png"}, {"balloons.png", 7, 6}, {"digits.png", 1, 10}, {"butterfly.png", 4, 10},
};

float Game::rando_rng(float menor, float mayor) {
	//Randzomiza el globo
	std::mt19937_64 gen(rd());
	std::uniform_real_distribution<float> dist(menor, mayor);
	rando = dist(gen);
	return rando;
}

void Game::sumarPuntosGlobo(int globosPrevios) { puntuacionActual += balpoints * (1 + globosPrevios); }
void Game::sumarFlechasCargadas() { numFlechasCargadas++; }
void Game::restarFlechasCargadas() { numFlechasCargadas--; }
void Game::restarGlobosCargados() { numGlobosCargados--; }
void Game::restarPuntosMariposa(int puntos) { { puntuacionActual -= puntos; } if (puntuacionActual < 0) { puntuacionActual = 0; }  }

int Game::getPuntuacion() const { return puntuacionActual; }

Texture* Game::getTexture(int i) {
	return textures[i];
}
	
int Game::getFlechas() const {
	if (bow != nullptr) {
		return bow->getFlechas();
	}
	return 0;
}
SDL_Renderer* Game::getRenderer() const { return renderer; }

inline Texture*
Game::getTexture(TextureName name) const
{
	return textures[name];
}

//Metodo par cargar atributos del archivo
bool Game::loadLevel(const string& n) {
	ifstream archivo(string (LEVEL_ROOT) + n);
	if (!archivo.is_open()) throw new FileNotFound(n);

	int línea = 0;
	try {

		while (objetosCargados.size() > 1) {
			delete objetosCargados.back();
			objetosCargados.pop_back();
		}
		numGlobosCargados = 0;
		numFlechasCargadas = 0;
		puntuacionActual = 0;

		string etiqueta;
		int r, g, b;
		

		while (archivo >> etiqueta) {
			línea++;
			if (etiqueta == "bgcolor")  archivo >> r >> g >> b;
			else if (etiqueta == "arrows")  archivo >> flechasIniciales;
			else if (etiqueta == "baldelay")  archivo >> balDelay;
			else if (etiqueta == "balpoints")  archivo >> balpoints; 
			else if (etiqueta == "goal")  archivo >> goal;
			else if (etiqueta == "butterflies")  archivo >> numMariposasIniciales;
		}

		colorFondoJuego.r = r;
		colorFondoJuego.g = g;
		colorFondoJuego.b = b;
		colorFondoJuego.a = 255;

		archivo.close();

		for (int i = 0; i < numMariposasIniciales; ++i) {
			Rectangle rect(Point2D<>(rando_rng(200.f, WINDOW_WIDTH - TAM_FRAME_MARIPOSA_X),
				rando_rng(60.f, WINDOW_HEIGHT - TAM_FRAME_MARIPOSA_Y)),
				TAM_FRAME_MARIPOSA_X, TAM_FRAME_MARIPOSA_Y);
			Vector2D<> vel(rando_rng(-2.f, 2.f), rando_rng(-2.f, 2.f));
			int color = static_cast<int>(rando_rng(0, 3));

			objetosCargados.push_back(new Butterfly(rect, vel, color, textures[BUTTERFLY], this));
		}

		if (bow != nullptr) {
			bow->setFlechas(flechasIniciales);
		}
	}
	catch (...) {
		throw new FileFormatError(n, línea);
	}


	return true;
}

Game::Game()
	: exit(false)
{
	// Carga SDL y crea la ventana con su renderizador
	if (!SDL_Init(SDL_INIT_VIDEO))
		throw "SDL_Init: "s + SDL_GetError();

	if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		0, &window, &renderer))
		throw "window or renderer: "s + SDL_GetError();

	// Carga las texturas al inicio
	try {
		textures.fill(nullptr);
		string imgRoot = IMAGE_ROOT;

		for (size_t i = 0; i < textures.size(); i++) {
			auto [name, nrows, ncols] = textureList[i];
			textures[i] = new Texture(renderer, (imgRoot + name).c_str(), nrows, ncols);
		}

		//Elige el archivo a cargar
		
	}

	catch (...) {
		// TODO: liberar la memoria reservada
		throw new SDLError();// relanza la excepción
	}

	

	//Crea el arco
	Point2D<> posArco(100.f, 300.f);
	Rectangle Arco(posArco, 64.f, 64.f);
	Vector2D<> velArco(0.f, 0.f);

	bow = new Bow(Arco, velArco, flechasIniciales, true, textures[1], this);

	objetosCargados.push_back(bow);

	//Crea la infobar
	infoBar = new InfoBar(0.f, 0.f, (float)WINDOW_WIDTH, 50.f, textures[DIGITS], this);

	loadLevel(to_string(nivelActual) + ".txt");


}

Game::~Game()
{
	//Eliminamos todo lo que sobre

	for (auto* object : objetosCargados) {
		delete object;
	}
	objetosCargados.clear();

	for (auto text : textures) {
		delete text;
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	// TODO: liberar la memoria reservada por la clase
}

void Game::loadSaveFile(const string& n) {
	ifstream archivo(string(SAVEFILE_ROOT) + n);

	if (!archivo.is_open()) throw std::runtime_error("No se pudo abrir el archivo de nivel: " + n);

	int newFlechas;

	string etiqueta;

	int linea = 0;
	try {
		while (archivo >> etiqueta) {
			linea++;
			if(etiqueta == "Puntuacion") archivo >> puntuacionActual;
			if (etiqueta == "Meta") archivo >> goal;
			if (etiqueta == "FlechasRestantes") { 
				archivo >> newFlechas; 
				bow->setFlechas(newFlechas); 
			}
		}
	}
	catch (...) {
		throw new FileFormatError(n, linea);
	}

}

void Game::saveGame() {
	ofstream archivo(string(SAVEFILE_ROOT) + "savefile.txt");
	archivo << "Puntacion" << puntuacionActual << endl;
	archivo <<  "Meta" << goal << endl;
	archivo << "FlechasRestantes" << bow->getFlechas() << endl;
}


void
Game::render() const
{
	SDL_SetRenderDrawColor(renderer, colorFondoJuego.r, colorFondoJuego.g, colorFondoJuego.b, colorFondoJuego.a);

	SDL_RenderClear(renderer);

	//Renderizamos todos los objetos
	for (SceneObject* object : objetosCargados) {
		object->render();
	}
	

	//Renderizamos la infobar
	if (infoBar != nullptr && bow != nullptr) {
		infoBar->render();
	}

	SDL_RenderPresent(renderer);
}

void
Game::update(float delta)
{
	//Bucle de generación de globos
	if (numGlobosCargados < NUM_GLOBOS_A_GENERAR && SDL_GetTicks() - time > balDelay) {
		// Generación de posición de inicio (horizontal) y velocidad del globo
		rando_rng(WINDOW_WIDTH / 3, WINDOW_WIDTH - TAM_FRAME_GLOBO_X);
		Point2D<> posGlobo(rando, WINDOW_HEIGHT);
		Rectangle Globo(posGlobo, TAM_FRAME_GLOBO_X, TAM_FRAME_GLOBO_Y);
		rando_rng(-4.f, -1.f);
		Vector2D<> velGlobo(0.f, rando);

		// Generación de color aleatorio
		rando_rng(0, 6);
		SceneObject* globo = new Balloon(Globo, velGlobo, rando, textures[3], this);

		// Inserción del globo en la lista deobjetos cargados de game
		objetosCargados.push_back(globo);
		numGlobosCargados++;

		// Reinicio del tiempo de delay
		time = SDL_GetTicks();
	}

	//Mueve flechas y las elimina
	for (auto it = objetosCargados.begin(); it != objetosCargados.end();) {

		if (!((*it)->update(delta))) {
			delete* it;
			it = objetosCargados.erase(it);
		}
		else {
			++it;
		}

	}

	if (infoBar != nullptr) {
		infoBar->update(delta);
	}

	//  El juego sigue en caso de conseguir el goal
	if (goal > 0 && puntuacionActual >= goal) {
		nivelActual++;
		if (!loadLevel(to_string(nivelActual) + ".txt")) {
			exit = true; // Activa el cierre limpio del bucle en Game::run
		}
	}

	// El arco no tiene flechas y no quedan flechas en el aire
	else if (bow->getFlechas() == 0 && numFlechasCargadas<=0) {
		exit = true; 
	}
}
void
Game::run()
{
	while (!exit) {

		update(0.016f);
		SDL_Delay(16);

		render();
		handleEvents();
	}
}



void
Game::handleEvents()
{
	SDL_Event event;
	//Maneja los evento de salida o de objetos
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT)
			exit = true;

		if (bow != nullptr) {
			bow->handleEvent(event);
		}

		// Eventos de carga y guardado de partida
		if (event.key.key == SDLK_O &&
			(event.key.mod & SDL_KMOD_CTRL)) {
			loadSaveFile("savefile.txt");
		}

		if (event.key.key == SDLK_S &&
			(event.key.mod & SDL_KMOD_CTRL)) {
			saveGame();
		}

	}
}

void Game::pushArrow(Arrow* flecha) {
	objetosCargados.push_back(flecha);
}

bool Game::checkCollision(const Rectangle& rect)
{
	//Comprueba la colisión
	for (SceneObject* object : objetosCargados) {
		Balloon* globo = dynamic_cast<Balloon*>(object);
		if (globo != nullptr && globo->Hit(rect)) {
			return true;
		}


		Butterfly* mariposa = dynamic_cast<Butterfly*>(object);
		if (mariposa != nullptr) {
			mariposa->Hit(rect);
		}
	}

	return false;
}


