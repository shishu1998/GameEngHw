#pragma once
#include "Entity.h"
class GameState {
public:
	int TextureID;
	void loadResources();
	void initEntities();
	void reset();
	void updateGameState(float elapsed);
};