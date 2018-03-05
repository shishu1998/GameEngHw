#define _USE_MATH_DEFINES
#include <math.h>
#include "GameState.h"
#include <algorithm>

void GameState::loadResources() {
	TextureID = LoadTexture(RESOURCE_FOLDER"sheet.png");
	playerShip = createSheetSprite(TextureID, 224, 832, 99, 75, 0.25);
	enemyShip = createSheetSprite(TextureID, 423, 728, 93, 84, 0.25);
	playerBullet = createSheetSprite(TextureID, 856, 421, 9, 54, 0.25);
	enemyBullet = createSheetSprite(TextureID, 858, 230, 9, 54, 0.25);
}
void GameState::initEntities() {
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
	playerLives.clear();
	playerBullets.clear();
	enemyBullets.clear();
	initEntities();
	score = 0;
}

void GameState::shootBullet(Entity& entity) {
	Entity bullet = Entity(entity.x, entity.type == Player ? entity.y + entity.height : entity.y - entity.height, Bullet, entity.type == Player ? playerBullet : enemyBullet);
	if (entity.type == Player) {
		bullet.velocity_y = 2.0f;
		bullet.rotation = 0;
		playerBullets.emplace_back(bullet);
	}
	else {
		bullet.velocity_y = -2.0f;
		bullet.rotation = M_PI;
		enemyBullets.emplace_back(bullet);
	}
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
				enemies[i][j].y -= 0.25f;
			}
		}
	}
	for (int i = 0; i < enemies.size(); ++i) {
		for (int j = 0; j < enemies[i].size(); ++j) {
			enemies[i][j].Move(elapsed);
		}
	}
	for (int i = 0; i < playerBullets.size(); ++i) {
		playerBullets[i].Move(elapsed);
		//Bullet to bullet collision
		for (int l = 0; l < enemyBullets.size(); ++l) {
			if (playerBullets[i].CollidesWith(enemyBullets[l])) {
				playerBullets[i].health -= 1;
				enemyBullets[l].health -= 1;
			}
		}
		//PlayerBullet to Enemy collision
		for (int j = 0; j < enemies.size(); ++j) {
			for (int k = 0; k < enemies[j].size(); ++k) {
				if (playerBullets[i].CollidesWith(enemies[j][k])) {
					enemies[j][k].health -= 1;
					playerBullets[i].health -= 1;
					score += 100;
				}
			}
		}
	}
	//EnemyBullet to player collision
	for (int i = 0; i < enemyBullets.size(); ++i) {
		enemyBullets[i].Move(elapsed);
		if (enemyBullets[i].CollidesWith(player)) {
			enemyBullets[i].health -= 1;
			player.health -= 1;
			playerLives.pop_back();
			score -= 500;
		}
	}

	playerBullets.erase(std::remove_if(playerBullets.begin(), playerBullets.end(), &GameState::shouldRemoveBullet), playerBullets.end());
	enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(), &GameState::shouldRemoveBullet), enemyBullets.end());
	for (int i = 0; i < enemies.size(); ++i) {
		enemies[i].erase(std::remove_if(enemies[i].begin(), enemies[i].end(), &GameState::shouldRemoveEnemy), enemies[i].end());
	}
	//If a column of enemies died, move every column to the right over
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