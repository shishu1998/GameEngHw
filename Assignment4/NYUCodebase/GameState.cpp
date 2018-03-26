#define _USE_MATH_DEFINES
#include "GameState.h"
#include "Helper.h"

//Loads the required resources for the entities
void GameState::loadResources() {
	TextureID = LoadTexture(RESOURCE_FOLDER"spritesheet_rgba.png");
	map.Load(levelFILE);
	for (int i = 0; i < map.entities.size(); i++) {
		PlaceEntity(map.entities[i].type, map.entities[i].x * tileSize, map.entities[i].y * -tileSize);
	}
	viewMatrix.Translate(-player.Position.x, -player.Position.y, 0);
}

//Initializes the Entities stored in the state
void GameState::initEntities() {
}

//Clear everything in the GameState and remake them, except the player, for the player simply set health = 3
void GameState::reset() {
}

//Updates the GameState based on the time elapsed
void GameState::updateGameState(float elapsed) {
	player.Update(elapsed);
	for (int i = 0; i < entities.size(); ++i) {
		entities[i].Update(elapsed);
		entities[i].CollidesWithTile(map.mapData, solidTiles);
		if (entities[i].collidedLeft) {
			entities[i].acceleration.x = 0.3;
		}
		if (entities[i].collidedRight) {
			entities[i].acceleration.x = -0.3;
		}
	}
	player.CollidesWithTile(map.mapData, solidTiles);
	for (int i = 0; i < entities.size(); ++i) {
		if (player.CollidesWith(entities[i])) 
			player.velocity.y = 0.6;
	}
	viewMatrix.Identity();
	viewMatrix.Translate(-player.Position.x, -player.Position.y, 0);
}

void GameState::PlaceEntity(std::string type, float x, float y)
{
	if (type == "Player") {
		player = Entity(x, y, createSheetSpriteBySpriteIndex(TextureID, 28, tileSize), Player, false);
	}
	else if (type == "Enemy") {
		Entity enemy = Entity(x, y, createSheetSpriteBySpriteIndex(TextureID, 446, tileSize), Enemy, false);
		enemy.acceleration.x = 0.3;
		entities.emplace_back(enemy);
	}
}

void GameState::Render(ShaderProgram & program)
{
	DrawLevel(program, TextureID, map, viewMatrix, 0.0, 0.0);
	player.Render(program, viewMatrix);
	for (int i = 0; i < entities.size(); ++i) {
		entities[i].Render(program, viewMatrix);
	}
}
