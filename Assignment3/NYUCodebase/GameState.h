#pragma once
#include "Entity.h"
class GameState {
public:
	int TextureID;
	SheetSprite playerShip;
	SheetSprite enemyShip;
	SheetSprite playerBullet;
	SheetSprite enemyBullet;
	Entity player;
	std::vector<Entity> playerLives;
	std::vector<std::vector<Entity>> enemies;
	std::vector<Entity> playerBullets;
	std::vector<Entity> enemyBullets;
	int score = 0;
	void initEntities();
	void reset();
	void shootBullet(Entity& entity);
	bool static shouldRemoveBullet(Entity bullet);
	bool static shouldRemoveEnemy(Entity enemy);
	void updateGameState(float elapsed);
};