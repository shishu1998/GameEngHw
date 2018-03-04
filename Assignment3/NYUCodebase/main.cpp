#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define STB_IMAGE_IMPLEMENTATION
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <math.h>
#include <algorithm>
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
std::vector<Entity> bullets;
float lastFrameTicks = 0.0f;
float elapsed = 0.0f;
float bulletCooldown = 1.0f;
float enemyBulletCooldown = 0.0f;

void initEntities() {
	program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	TextureID = LoadTexture(RESOURCE_FOLDER"sheet.png");
	fontTextureID = LoadTexture(RESOURCE_FOLDER"font1.png");

	playerShip = createSheetSprite(TextureID, 224, 832, 99, 75, 0.25);
	enemyShip = createSheetSprite(TextureID, 423, 728, 93, 84, 0.25);
	playerBullet = createSheetSprite(TextureID, 856, 421, 9, 54, 0.25);
	enemyBullet = createSheetSprite(TextureID, 858, 230, 9, 54, 0.25);

	float shipWidth = enemyShip.width * enemyShip.size / enemyShip.height;
	player = Entity(0.0f - shipWidth / 2, -1.5f, Player, playerShip);

	for (int i = 0; i < 3; ++i) {
		playerLives.emplace_back(-2.0f + i * 0.3f, -1.85f, Life, playerShip);
	}

	for (int i = 0; i < 8; ++i) {
		enemies.emplace_back(std::vector<Entity>());
		for (int j = 0; j < 3; ++j) {
			Entity enemy = Entity(-shipWidth * 8 + i * shipWidth * 2, 1.8 - j * enemyShip.size * 2, Enemy, enemyShip);
			enemy.velocity_x = 0.5f;
			enemies.back().push_back(enemy);
		}
	}
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
	state = Start;

	initEntities();
}

void reset() {
	initEntities();
	bullets.clear();
	state = Start;
}

void shootBullet(Entity& entity) {
	Entity bullet = Entity(entity.x, entity.type == Player ? entity.y + entity.height : entity.y - entity.height, Bullet, entity.type == Player ? playerBullet : enemyBullet);
	if (entity.type == Player) {
		bullet.velocity_y = 1.0f;
		bullet.rotation = 0;
	}
	else {
		bullet.velocity_y = -1.0f;
		bullet.rotation = M_PI;
	}
	bullets.emplace_back(bullet);
}

bool shouldRemoveBullet(Entity bullet) {
	return bullet.height / 2 + bullet.y > 2.0 || bullet.y - bullet.height / 2 < -1.85 || bullet.health < 1;
}

bool shouldRemoveEnemy(Entity enemy) {
	return enemy.health < 1;
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

	if (keys[SDL_SCANCODE_J] && bulletCooldown >= 1.0) {
		shootBullet(player);
		bulletCooldown = 0.0f;
	}
}

void updateGameState(float elapsed) {
	player.Move(elapsed);
	if (enemies[0][0].x - enemies[0][0].width / 2 < -3.55 || enemies.back()[0].x + enemies.back()[0].width / 2 > 3.55) {
		for (int i = 0; i < enemies.size(); ++i) {
			for (int j = 0; j < enemies[i].size(); ++j) {
				enemies[i][j].velocity_x *= -1;
				enemies[i][j].y -= 0.5f;
			}
		}
	}
	for (int i = 0; i < enemies.size(); ++i) {
		for (int j = 0; j < enemies[i].size(); ++j) {
			enemies[i][j].Move(elapsed);
		}
	}
	for (int i = 0; i < bullets.size(); ++i) {
		bullets[i].Move(elapsed);
		for (int j = 0; j < enemies.size(); ++j) {
			for (int k = 0; k < enemies[j].size(); ++k) {
				if (bullets[i].CollidesWith(enemies[j][k])) {
					enemies[j][k].health -= 1;
					bullets[i].health -= 1;
				}
			}
		}
		if (bullets[i].CollidesWith(player)) {
			bullets[i].health -= 1;
			player.health -= 1;
			playerLives.pop_back();
		}
	}
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), shouldRemoveBullet), bullets.end());
	for (int i = 0; i < enemies.size(); ++i) {
		enemies[i].erase(std::remove_if(enemies[i].begin(), enemies[i].end(), shouldRemoveEnemy), enemies[i].end());
	}
	for (int i = 0; i < enemies.size(); ++i) {
		if (enemies[i].empty()) {
			for (int j = i; j < enemies.size() - 1; ++j) {
				enemies[j] = enemies[j + 1];
				for (int k = 0; k < enemies[j].size(); ++k) {
					enemies[j][k].x -= 2 * enemies[j][k].width;
				}
			}
			enemies.pop_back();
		}
	}
}

void renderGame() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	bulletCooldown += elapsed;
	enemyBulletCooldown += elapsed;
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

	srand(SDL_GetTicks());
	int randEnemyShoot = rand();
	if (enemyBulletCooldown > 2.0f / enemies.size()) {
		shootBullet(enemies[randEnemyShoot%enemies.size()].back());
		enemyBulletCooldown = 0.0f;
	}

	for (int i = 0; i < bullets.size(); ++i) {
		bullets[i].Draw(program);
	}
	if (player.health < 1) {
		state = Defeat;
	}
	if (enemies.empty()) {
		state = Victory;
	}
	for (int i = 0; i < enemies.size(); ++i) {
		if (enemies[i].back().y - enemies[i].back().height/2 < -1.85) {
			state = Defeat;
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
			reset();
		}
		break;
	case Defeat:
		DrawMessage(program, fontTextureID, "YOU LOSE - PRESS SPACE", -0.3 * 22 / 2.0, 0.5, 0.3f, 0.0f);
		DrawMessage(program, fontTextureID, "TO REPLAY THE GAME", -0.3 * 18 / 2.0, -0.5, 0.3f, 0.0f);
		if (keys[SDL_SCANCODE_SPACE]) {
			reset();
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
