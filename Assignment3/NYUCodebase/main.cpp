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

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;
ShaderProgram program;
enum GameState { Start, Game, Victory, Defeat};

void init() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 960, 540);
	program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
}

void DrawStart() {
	Matrix modelMatrix;
	Matrix viewMatrix;
	Matrix projectionMatrix;
	projectionMatrix.SetOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);

	std::string text = "PRESS SPACE";
	float size = 0.3f;
	float space = 0.0f;
	//centering
	modelMatrix.Translate(-(size + space) * text.size() / 2, 0.5, 0);

	program.SetModelMatrix(modelMatrix);
	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);
	int TextureID = LoadTexture(RESOURCE_FOLDER"font1.png");
	DrawText(&program, TextureID, text, size, space);

	text = "TO START";
	Matrix modelMatrix2;
	modelMatrix2.Translate(-(size + space) * text.size() / 2, -0.5, 0);
	program.SetModelMatrix(modelMatrix2);
	DrawText(&program, TextureID, text, size, space);
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
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(program.programID);

		DrawStart();

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}
