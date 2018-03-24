#define _USE_MATH_DEFINES
#include "GameState.h"
#include "Helper.h"
#define levelFILE "Arne.txt"

//Loads the required resources for the entities
void GameState::loadResources() {
	TextureID = LoadTexture(RESOURCE_FOLDER"arne_sprites.png");
	map.Load(levelFILE);
	for (int i = 0; i < map.entities.size(); i++) {
		PlaceEntity(map.entities[i].type, map.entities[i].x * tileSize, map.entities[i].y * -tileSize);
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

void GameState::PlaceEntity(std::string type, float x, float y)
{
	if (type == "Player") {
		player = Entity(x, y, createSheetSpriteBySpriteIndex(TextureID, 98, tileSize), Player);
	}
	else {
		
	}
}
