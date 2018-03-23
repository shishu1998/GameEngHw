#define _USE_MATH_DEFINES
#include "GameState.h"
#define levelFILE "Arne.txt"
#define tileSize 0.5f

//Loads the required resources for the entities
void GameState::loadResources() {
	TextureID = LoadTexture(RESOURCE_FOLDER"arne_sprites.png");
	map.Load(levelFILE);
	for (int y = 0; y < map.mapHeight; y++) {
		for (int x = 0; x < map.mapWidth; x++) {
			if(map.mapData[y][x] != 0) PlaceEntity(map.mapData[y][x], -3.525f + x * 0.05, 0 - y*0.05);
		}
	}
}

//Initializes the Entities stored in the state
void GameState::initEntities() {
}

//Clear everything in the GameState and remake them, except the player, for the player simply set health = 3
void GameState::reset() {
}

//Updates the GameState based on the time elapsed
void GameState::updateGameState(float elapsed) {
}

void GameState::PlaceEntity(int spriteIndex, float x, float y)
{
	int spriteSheetx = spriteIndex % 16;
	int spriteSheetY = spriteIndex / 16;
	SheetSprite sprite = createSheetSprite(TextureID, spriteSheetx * 16.0f, spriteSheetY * 16.0f, 16.0f, 16.0f, 0.1);
	entities.emplace_back(x, y, sprite);
}
