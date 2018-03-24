#pragma once
#include "Entity.h"
#include "FlareMap.h"
#include <vector>

class GameState {
public:
	int TextureID;
	FlareMap map;
	std::vector<Entity> entities;
	Entity player;
	Matrix viewMatrix;
	void loadResources();
	void initEntities();
	void reset();
	void updateGameState(float elapsed);
	void PlaceEntity(std::string type, float x, float y);
	void Render(ShaderProgram &program);
};