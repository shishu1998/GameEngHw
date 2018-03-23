#pragma once
#include "Entity.h"
#include "FlareMap.h"
#include <vector>

class GameState {
public:
	int TextureID;
	FlareMap map;
	std::vector<Entity> entities;
	void loadResources();
	void initEntities();
	void reset();
	void updateGameState(float elapsed);
	void PlaceEntity(int spriteIndex, float x, float y);
};