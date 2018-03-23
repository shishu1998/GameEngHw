#define _USE_MATH_DEFINES
#include "GameState.h"
#define levelFILE "Arne.txt"
#define tileSize 0.5f

//Loads the required resources for the entities
void GameState::loadResources() {
	map.Load(levelFILE);
	for (int y = 0; y < map.mapHeight; y++) {
		for (int x = 0; x < map.mapWidth; x++) {
			// do something with map.mapData[y][x] 
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

void GameState::PlaceEntity(std::string type, float x, float y)
{
	entities.emplace_back();
}
