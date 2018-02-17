#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"
#include "Board.h"

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;
bool done = false;
float p1Y = 0;
float p2Y = 0;

void setup() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif
}

void ProcessEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_W:
					p1Y = 0.001;
					break;
				case SDL_SCANCODE_S:
					p1Y = -0.001;
					break;
				case SDL_SCANCODE_UP:
					p2Y = 0.001;
					break;
				case SDL_SCANCODE_DOWN:
					p2Y = -0.001;
					break;
				default:
					break;
			}
		}
		else if (event.type == SDL_KEYUP) {
			switch (event.key.keysym.scancode) {
			case SDL_SCANCODE_W:
			case SDL_SCANCODE_S:
				p1Y = 0;
				break;
			case SDL_SCANCODE_UP:
			case SDL_SCANCODE_DOWN:
				p2Y = 0;
				break;
			default:
				break;
			}
		}
	}
}

int main(int argc, char *argv[])
{
	setup();
	Board p1(-3.525, 0, 0.05 , 1);
	Board p2(3.525, 0, 0.05, 1);
	ShaderProgram Program;
	Program.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	float lastFrameTicks = 0.0f;
	float elapsed = 0.0f;
	while (!done) {
		glClear(GL_COLOR_BUFFER_BIT);

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		elapsed += ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		if (elapsed > 0.1) {
			ProcessEvents();
			elapsed = 0.0f;
		}

		p1.y += p1Y;
		p2.y += p2Y;

		Program.SetColor(1, 0, 0, 1);
		p1.Draw(Program);
		Program.SetColor(0, 1, 0, 1);
		p2.Draw(Program);

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}
