#define _USE_MATH_DEFINES
#include <math.h>
#include "GameState.h"
#include <algorithm>
void GameState::initEntities() {
	TextureID = LoadTexture(RESOURCE_FOLDER"sheet.png");

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

void GameState::reset() {
	enemies.clear();
	bullets.clear();
	initEntities();
}

void GameState::shootBullet(Entity& entity) {
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

bool GameState::shouldRemoveBullet(Entity bullet) {
	return bullet.height / 2 + bullet.y > 2.0 || bullet.y - bullet.height / 2 < -1.85 || bullet.health < 1;
}

bool GameState::shouldRemoveEnemy(Entity enemy) {
	return enemy.health < 1;
}

void GameState::updateGameState(float elapsed) {
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
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), &GameState::shouldRemoveBullet), bullets.end());
	for (int i = 0; i < enemies.size(); ++i) {
		enemies[i].erase(std::remove_if(enemies[i].begin(), enemies[i].end(), &GameState::shouldRemoveEnemy), enemies[i].end());
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