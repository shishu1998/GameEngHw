#pragma once
#include "Entity.h"
#include "FlareMap.h"
#include <vector>
#include <unordered_map>
#define Solids {{63,1},{64,1},{65,1}, {69,1}, {92,1}, {93,1}, {95,1}}

class GameState {
public:
	int TextureID;
	FlareMap map;
	std::vector<Entity> entities;
	std::unordered_map<int, int> solidTiles = std::unordered_map<int,int>(Solids);
	Entity player;
	Matrix viewMatrix;
	void loadResources();
	void initEntities();
	void reset();
	void updateGameState(float elapsed);
	void PlaceEntity(std::string type, float x, float y);
	void Render(ShaderProgram &program);
};