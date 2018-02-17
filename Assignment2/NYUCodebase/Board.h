#pragma once
#include "ShaderProgram.h"

class Board
{
public:
	Board(float x, float y, float width, float height);
	void Draw(ShaderProgram &Program) const;
	float x;
	float y;
	float rotation = 0;
	int textureID;
	float width = 1;
	float height = 3;
};

