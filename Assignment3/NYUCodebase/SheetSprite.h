#pragma once
#include "ShaderProgram.h"

class SheetSprite {
public:
	SheetSprite(unsigned int textureID, float u, float v, float width, float height, float
		size);
	void Draw(ShaderProgram *program);
	float size;
	unsigned int textureID;
	float u;
	float v;
	float width;
	float height;
};