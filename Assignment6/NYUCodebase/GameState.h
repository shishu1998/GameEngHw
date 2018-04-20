#pragma once
#include "Entity.h"
#include "FlareMap.h"
#include <vector>
#include <SDL_mixer.h>
#define levelFILE "Alien.txt"
#define Solids {61,62,68,92,94,490,579}

class GameState {
public:
	int TextureID;
	FlareMap map;
	std::vector<Entity> entities;
	std::unordered_set<int> solidTiles = std::unordered_set<int>(Solids);
	Entity player;
	Matrix viewMatrix;
	Mix_Chunk *collide;
	Mix_Chunk *scream;
	Mix_Music *bgm;
	void loadResources();
	void initEntities();
	void reset();
	void updateGameState(float elapsed);
	void PlaceEntity(std::string type, float x, float y);
	void Render(ShaderProgram &program);
	void GameState::playBackgroundMusic() const;
};