#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <math.h>
#include "ShaderProgram.h"
#include "Matrix.h"
#include "Entity.h"

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;
bool done = false;
Entity p1(-3.525f, 0.0f, 0.05f, 1.0f);
Entity p2(3.525f, 0.0f, 0.05f, 1.0f);
Entity pong(0, 0, 0.1, 0.1);
float lastFrameTicks = 0.0f;
float elapsed = 0.0f;
int winner = 0;

void setup() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif
	
	pong.velX = 0.001f;
	pong.velY = 0.005f;
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
					p1.velY = 0.005f;
					break;
				case SDL_SCANCODE_S:
					p1.velY = -0.005f;
					break;
				case SDL_SCANCODE_UP:
					p2.velY = 0.005f;
					break;
				case SDL_SCANCODE_DOWN:
					p2.velY = -0.005f;
					break;
				default:
					break;
			}
		}
		else if (event.type == SDL_KEYUP) {
			switch (event.key.keysym.scancode) {
			case SDL_SCANCODE_W:
			case SDL_SCANCODE_S:
				p1.velY = 0;
				break;
			case SDL_SCANCODE_UP:
			case SDL_SCANCODE_DOWN:
				p2.velY = 0;
				break;
			default:
				break;
			}
		}
	}
}

void TimedEvents() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	elapsed += ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	if (elapsed > 0.005) {
		ProcessEvents();
		if ((p1.velY > 0 && p1.y + p1.height / 2 < 2.0) || (p1.velY < 0 && p1.y - p1.height / 2 > -2.0))
			p1.Move();
		if ((p2.velY > 0 && p2.y + p2.height / 2 < 2.0) || (p2.velY < 0 && p2.y - p2.height / 2 > -2.0))
			p2.Move();
		elapsed = 0.0f;
	}
}

void PongMovement() {
	if (!((pong.velY > 0 && pong.y + pong.height / 2 < 2.0) || (pong.velY < 0 && pong.y - pong.height / 2 > -2.0)))
		pong.velY *= -1.0f;

	if (pong.Collision(p1) || pong.Collision(p2)) {
		pong.velX *= -1.0f;
	}
	if (pong.x - pong.width > 3.55f) winner = 1;
	if (pong.x + pong.width < -3.55f) winner = 2;
	pong.Move();
}

int main(int argc, char *argv[])
{
	setup();
	ShaderProgram Program;
	Program.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	while (!done) {
		while (!winner) {
			glClear(GL_COLOR_BUFFER_BIT);

			TimedEvents();
			Program.SetColor(1, 0, 0, 1);
			p1.Draw(Program);
			Program.SetColor(0, 1, 0, 1);
			p2.Draw(Program);
			Program.SetColor(1, 1, 1, 1);
			PongMovement();
			pong.Draw(Program);

			SDL_GL_SwapWindow(displayWindow);
		}
	}

	SDL_Quit();
	return 0;
}
