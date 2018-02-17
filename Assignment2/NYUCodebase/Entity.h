#pragma once
#include "ShaderProgram.h"

class Entity
{
public:
	Entity(float x, float y, float width, float height);
	void Draw(ShaderProgram &Program) const;
	bool Collision(const Entity& Other) const;
	void Move();
	float x;
	float y;
	float velX = 0;
	float velY = 0;
	float rotation = 0;
	int textureID;
	float width = 1;
	float height = 3;
};

