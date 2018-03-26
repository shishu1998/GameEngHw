#define _USE_MATH_DEFINES
#include "GameState.h"
#include "Helper.h"
#define levelFILE "Alien.txt"

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
	int playerGridX, playerGridY, playerGridLeft, playerGridRight, playerGridTop, playerGridBottom;
	worldToTileCoordinates(player.Position.x, player.Position.y, &playerGridX, &playerGridY);
	worldToTileCoordinates(player.Position.x - player.size.x /2, player.Position.y - player.size.y / 2, &playerGridLeft, &playerGridBottom);
	worldToTileCoordinates(player.Position.x + player.size.x/2, player.Position.y + player.size.y / 2, &playerGridRight, &playerGridTop);
	if (map.mapData[playerGridTop][playerGridX] != 0) player.CollideTop(playerGridTop);
	if (map.mapData[playerGridBottom][playerGridX] != 0) player.CollideBottom(playerGridBottom);
	if (map.mapData[playerGridY][playerGridLeft] != 0) player.CollideLeft(playerGridLeft);
	if (map.mapData[playerGridY][playerGridRight] != 0) player.CollideRight(playerGridRight);
	viewMatrix.Identity();
	viewMatrix.Translate(-player.Position.x, -player.Position.y, 0);
}

void GameState::PlaceEntity(std::string type, float x, float y)
{
	if (type == "Player") {
		player = Entity(x, y, createSheetSpriteBySpriteIndex(TextureID, 28, tileSize), Player, false);
	}
	else if (type == "Enemy") {
		entities.emplace_back(Entity(x, y, createSheetSpriteBySpriteIndex(TextureID, 58, tileSize), Enemy, false));
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
