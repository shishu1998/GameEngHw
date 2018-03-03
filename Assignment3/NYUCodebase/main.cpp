#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define STB_IMAGE_IMPLEMENTATION
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Helper.h"
#include "Matrix.h"
#include "Entity.h"
#include "SheetSprite.h"

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;
ShaderProgram program;
enum GameState { Start, Game, Victory, Defeat};
GameState state;
const Uint8 *keys = SDL_GetKeyboardState(nullptr);

void init() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 960, 540);
	state = Start;
	program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
}

void renderGame() {

}

// Normalize the coordinates of the sprite sheet and create a sheetsp
SheetSprite& createSheetSprite(unsigned int textureID, float x, float y, float width, float height, float size) {
	return SheetSprite(textureID, x / 1024, y / 1024, width / 1024, height / 1024, size);
}

void renderState() {
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(program.programID);
	switch (state) {
	case Start:
		DrawMessage(program, "PRESS SPACE", "TO START");
		if (keys[SDL_SCANCODE_SPACE]) {
			state = Game;
		}
		break;
	case Game:
		renderGame();
		break;
	case Victory:
		DrawMessage(program, "YOU WIN - PRESS SPACE", "TO REPLAY THE GAME");
		if (keys[SDL_SCANCODE_SPACE]) {
			state = Game;
		}
		break;
	case Defeat:
		DrawMessage(program, "YOU LOSE - PRESS SPACE", "TO REPLAY THE GAME");
		if (keys[SDL_SCANCODE_SPACE]) {
			state = Game;
		}
		break;
	}
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
		renderState();
		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}
