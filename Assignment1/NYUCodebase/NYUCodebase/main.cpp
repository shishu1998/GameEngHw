#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#define STB_IMAGE_IMPLEMENTATION
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "stb_image.h"
#include "ShaderProgram.h"
#include "Matrix.h"

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

GLuint LoadTexture(const char *filePath) {
	int w, h, comp;
	unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);
	if (image == NULL) {
		std::cout << "Unable to load image. Make sure the path is correct\n";
		assert(false);
	}
	GLuint retTexture;
	glGenTextures(1, &retTexture);
	glBindTexture(GL_TEXTURE_2D, retTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(image);
	return retTexture;
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif

	glViewport(0, 0, 960, 540);
	ShaderProgram program;
	program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	ShaderProgram untexturedProgram;
	untexturedProgram.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");

	Matrix projectionMatrix;
	Matrix modelMatrix;
	modelMatrix.Translate(-2.5, -1.5, 0);
	Matrix modelMatrix2;
	modelMatrix2.Translate(-2.5, -0.5, 0);
	Matrix modelMatrix3;
	modelMatrix3.Scale(0.25, 0.25, 1);
	Matrix modelMatrix4;
	modelMatrix4.Translate(2.5, 1.5, 0);
	Matrix modelMatrix5;
	Matrix viewMatrix;
	Matrix viewMatrix2;

	projectionMatrix.SetOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);

	float lastFrameTicks = 0.0f;
	int move = 0;
	float moveTime = 0.0f;
	bool moveDown = false;
	bool expand = true;

	SDL_Event event;
	bool done = false;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}	
		}
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(program.programID);

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;


		program.SetModelMatrix(modelMatrix5);
		LoadTexture(RESOURCE_FOLDER"backgrounds.png");
		program.SetProjectionMatrix(projectionMatrix);
		program.SetViewMatrix(viewMatrix);
		
		//Draws the background over the entire screen
		float bgvertices[] = { -3.55, -2.0, 3.55, -2.0, 3.55, 2.0, -3.55, -2.0, 3.55, 2.0, -3.55, 2.0 };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, bgvertices);
		glEnableVertexAttribArray(program.positionAttribute);

		float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		//converts the vertices back to regular polygon
		glDisableVertexAttribArray(program.positionAttribute);
		float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program.positionAttribute);
		
		//Loads and draws Pipimi
		program.SetModelMatrix(modelMatrix);
		LoadTexture(RESOURCE_FOLDER"MetallicPipimi.png");

		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Loads and draws climbing Popuko
		program.SetModelMatrix(modelMatrix2);
		LoadTexture(RESOURCE_FOLDER"PopukoClimb.png");
		moveTime += elapsed;
		//Check every tenth of a second
		if (moveTime > 0.1) {
			moveTime = 0.0f;
			move += moveDown ? -1 : 1;
			modelMatrix2.Translate(0, moveDown ? -0.01 : 0.01, 0);
			//Only move each direction 100 times max
			if (move >= 100 || move <= 0) {
				moveDown = !moveDown;
			}
		}

		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Loads and draws dank
		program.SetModelMatrix(modelMatrix4);
		expand ? modelMatrix4.Scale(1.5, 1.5, 1) : modelMatrix4.Scale(2.0/3.0, 2.0/3.0, 1);
		//Expand/Shrink every other frame
		expand = !expand;
		LoadTexture(RESOURCE_FOLDER"dank.png");
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);
		
		//Draws the untextured Shuriken
		glUseProgram(untexturedProgram.programID);
		untexturedProgram.SetColor(1, 0, 0, 1);
		untexturedProgram.SetModelMatrix(modelMatrix3);
		untexturedProgram.SetProjectionMatrix(projectionMatrix);
		untexturedProgram.SetViewMatrix(viewMatrix2);

		viewMatrix2.Rotate(elapsed);

		float vertices2[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, -2.0, 0.0, -0.5, -0.5, 0.5, 0.5, 0.0, 2.0, -0.5, 0.5, 0.5, -0.5, 2.0, 0.0, 0.5, 0.5, 0.5, -0.5, 0.0, -2.0, -0.5, -0.5 };
		glVertexAttribPointer(untexturedProgram.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
		glEnableVertexAttribArray(untexturedProgram.positionAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 18);
		glDisableVertexAttribArray(untexturedProgram.positionAttribute);

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}
