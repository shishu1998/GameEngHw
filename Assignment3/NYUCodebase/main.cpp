#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define STB_IMAGE_IMPLEMENTATION
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <math.h>
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
GameState state;
const Uint8 *keys = SDL_GetKeyboardState(nullptr);
int TextureID;
int fontTextureID;
SheetSprite playerShip;
SheetSprite enemyShip;
SheetSprite playerBullet;
SheetSprite enemyBullet;
Entity player;
std::vector<Entity> playerLives;
std::vector<std::vector<Entity>> enemies;
float lastFrameTicks = 0.0f;
float elapsed = 0.0f;

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
	TextureID = LoadTexture(RESOURCE_FOLDER"sheet.png");
	fontTextureID = LoadTexture(RESOURCE_FOLDER"font1.png");

	playerShip = createSheetSprite(TextureID, 224, 832, 99, 75, 0.25);
	enemyShip = createSheetSprite(TextureID, 423, 728, 93, 84, 0.25);
	playerBullet = createSheetSprite(TextureID, 856, 421, 9, 54, 0.25);
	enemyBullet = createSheetSprite(TextureID, 858, 230, 9, 54, 0.25);

	float shipWidth = enemyShip.width * enemyShip.size / enemyShip.height;
	player = Entity(0.0f - shipWidth/2, -1.5f, Player, playerShip);
	
	for (int i = 0; i < 3; ++i) {
		playerLives.emplace_back(-2.0f + i * 0.3f, -1.85f, Life, playerShip);
	}

	for (int i = 0; i < 3; ++i) {
		enemies.emplace_back(std::vector<Entity>());
		for (int j = 0; j < 20; ++j) {
			Entity enemy = Entity(-shipWidth/2 * 20 + j * shipWidth, 1.8 - i * enemyShip.size, Enemy, enemyShip);
			enemies.back().push_back(enemy);
		}
	}
}

void processGameState() {
	if (keys[SDL_SCANCODE_A]) {
		player.velocity_x = -1.0f;
	}
	else if (keys[SDL_SCANCODE_D]) {
		player.velocity_x = 1.0f;
	}
	else {
		player.velocity_x = 0.0f;
	}
}

void updateGameState(float elapsed) {
	player.Move(elapsed);
}

void renderGame() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	processGameState();
	updateGameState(elapsed);
	player.Draw(program);
	DrawMessage(program, fontTextureID, "LIVES:", -3.4, -1.85, 0.2, 0.0);
	for (int i = 0; i < playerLives.size(); ++i) {
		playerLives[i].Draw(program);
	}
	for (int i = 0; i < enemies.size(); ++i) {
		for (int j = 0; j < enemies[i].size(); ++j) {
			enemies[i][j].Draw(program);
		}
	}
}

void renderState() {
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(program.programID);
	switch (state) {
	case Start:
		DrawMessage(program, fontTextureID, "PRESS SPACE TO START", -0.3 * 19/2.0, 0.5, 0.3f, 0.0f);
		DrawMessage(program, fontTextureID, "INSTRUCTIONS", -0.2*12 /2.0, 0.0f, 0.2f, 0.0f);
		DrawMessage(program, fontTextureID, "USE A AND D TO MOVE", -0.15 * 19/ 2.0, -0.2, 0.15f, 0.0f);
		DrawMessage(program, fontTextureID, "USE J TO SHOOT", -0.15 * 14 / 2.0, -0.35, 0.15f, 0.0f);
		if (keys[SDL_SCANCODE_SPACE]) {
			state = Game;
		}
		break;
	case Game:
		renderGame();
		break;
	case Victory:
		DrawMessage(program, fontTextureID, "YOU WIN - PRESS SPACE", -0.3 * 21 / 2.0, 0.5, 0.3f, 0.0f);
		DrawMessage(program, fontTextureID, "TO REPLAY THE GAME", -0.3 * 18 / 2.0, -0.5, 0.3f, 0.0f);
		if (keys[SDL_SCANCODE_SPACE]) {
			state = Game;
		}
		break;
	case Defeat:
		DrawMessage(program, fontTextureID, "YOU LOSE - PRESS SPACE", -0.3 * 22 / 2.0, 0.5, 0.3f, 0.0f);
		DrawMessage(program, fontTextureID, "TO REPLAY THE GAME", -0.3 * 18 / 2.0, -0.5, 0.3f, 0.0f);
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
