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
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

SDL_Window* displayWindow;
ShaderProgram program;
ShaderProgram untexturedProgram;
Matrix projectionMatrix;
Matrix viewMatrix;
const Uint8 *keys = SDL_GetKeyboardState(nullptr);
int fontTextureID;
float lastFrameTicks = 0.0f;
float elapsed = 0.0f;
float accumulator = 0.0f;
GameState state;
std::vector<Entity> staticEntities;
Entity one, two, three;

#pragma region "Assignment 5 Code"
void EntitySetup() {
	staticEntities.emplace_back(-3.55, 0, 0.1, 4.0, true);
	staticEntities.emplace_back(3.55, 0, 0.1, 4.0, true);
	staticEntities.emplace_back(0, 2.0, 7.1, 0.1, true);
	staticEntities.emplace_back(0, -2.0, 7.1, 0.1, true);
	one = Entity(-1.0, 0, 0.5, 1.0, false);
	one.velocity = Vector4(-0.1, 0.1,0);
	one.Rotate(M_PI / 4);
	two = Entity(1.0, 0.5, 1.0, 0.5, false);
	two.velocity = Vector4(-0.1, -0.1, 0);
	two.Rotate(5*M_PI/4);
	three = Entity(0, 1.0, 0.25, 0.25, false);
	three.velocity = Vector4(0.1, -0.1, 0);
	three.Rotate(1.0);
}

void RenderEntities() {
	for (int i = 0; i < staticEntities.size(); ++i) {
		staticEntities[i].Render(untexturedProgram, viewMatrix);
	}
	one.Render(untexturedProgram, viewMatrix);
	two.Render(untexturedProgram, viewMatrix);
	three.Render(untexturedProgram, viewMatrix);
}

void SetRandomVelocity(Entity& entity) {
	entity.velocity.x = ((float)rand() / RAND_MAX - 0.5);
	entity.velocity.y = (float)rand() / RAND_MAX - 0.5;
}

void UpdateEntities(float elapsed) {
	for (int i = 0; i < staticEntities.size(); ++i) {
		if (one.SATCollidesWith(staticEntities[i])) {
			SetRandomVelocity(one);
		}
		if (two.SATCollidesWith(staticEntities[i])) {
			SetRandomVelocity(two);
		}
		if (three.SATCollidesWith(staticEntities[i])) {
			SetRandomVelocity(three);
		}
	}
	if (one.SATCollidesWith(two)) {
		SetRandomVelocity(one);
		SetRandomVelocity(two);
	}
	if (one.SATCollidesWith(three)) {
		SetRandomVelocity(one);
		SetRandomVelocity(three);
	}
	if (two.SATCollidesWith(three)) {
		SetRandomVelocity(two);
		SetRandomVelocity(three);
	}
	//Won't move since they're static, I put this here to test my static check in update
	for (int i = 0; i < staticEntities.size(); ++i) {
		staticEntities[i].Update(elapsed);
	}
	one.Update(elapsed);
	two.Update(elapsed);
	three.Update(elapsed);
}

#pragma endregion

void init() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif

	program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	untexturedProgram.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	glViewport(0, 0, 960, 540);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	projectionMatrix.SetOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
	program.SetProjectionMatrix(projectionMatrix);
	untexturedProgram.SetProjectionMatrix(projectionMatrix);

	EntitySetup();
	srand(SDL_GetTicks());

	glUseProgram(program.programID);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	state.loadResources();
}

void processGameState(GameState& state) {
	if (keys[SDL_SCANCODE_A]) {
		state.player.acceleration.x = -0.2;
		state.player.forward = false;
	}
	if (keys[SDL_SCANCODE_D]) {
		state.player.acceleration.x = 0.2;
		state.player.forward = true;
	}
	if (keys[SDL_SCANCODE_SPACE]) {
		if(state.player.collidedBottom)	state.player.velocity.y = 0.4;
	}
	if(!(keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_SPACE])) {
		state.player.acceleration.x = 0.0;
		state.player.acceleration.y = 0.0;
	}
}

void updateGameState(GameState& state, float elapsed) {
	state.updateGameState(elapsed);
}

int main(int argc, char *argv[])
{
	init();
	SDL_Event event;
	bool done = false;
	untexturedProgram.SetColor(1.0, 0.0, 0, 1);
	while (!done) {
		glClear(GL_COLOR_BUFFER_BIT);
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		elapsed += accumulator;
		if (elapsed < FIXED_TIMESTEP) {
			accumulator = elapsed;
			UpdateEntities(FIXED_TIMESTEP);
			continue;
		}
		while (elapsed >= FIXED_TIMESTEP) {
			elapsed -= FIXED_TIMESTEP;
		}
		RenderEntities();
		accumulator = elapsed;
		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}
