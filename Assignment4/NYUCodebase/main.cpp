#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define STB_IMAGE_IMPLEMENTATION
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "GameState.h"

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;
ShaderProgram program;
const Uint8 *keys = SDL_GetKeyboardState(nullptr);
int fontTextureID;
float lastFrameTicks = 0.0f;
float elapsed = 0.0f;

void initEntities(GameState& state) {
}

void reset(GameState& state) {
}


void init() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 960, 540);
}

void processGameState(GameState& state) {
	if (keys[SDL_SCANCODE_A]) {
	}
	else if (keys[SDL_SCANCODE_D]) {
	}
	else {
	}
}

void updateGameState(GameState& state, float elapsed) {
	state.updateGameState(elapsed);
}

void renderGame(GameState& state) {
}

void renderState(GameState& state) {
}

int main(int argc, char *argv[])
{
	init();
	SDL_Event event;
	bool done = false;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}
