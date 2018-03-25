#pragma once
#include "Entity.h"
#include "FlareMap.h"
#include <vector>
#include <unordered_map>
#define Solids {{17,1},{19,1},{33,1},{91,1},{92,1},{93,1},{94,1},{107,1},{108,1},{109,1},{110,1},{121,1}, {122,1}, {123,1},{124,1},{125,1},{126,1}}

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