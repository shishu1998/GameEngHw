#pragma once
#include "ShaderProgram.h"

class Entity {
public:
	Entity(float x, float y, float width, float height);
	void Draw(ShaderProgram &Program);
	bool CollidesWith(const Entity& Other) const;
	void Move();
	float x;
	float y;
	float rotation;
	int textureID;
	float width;
	float height;
	float velocity_x;
	float velocity_y;
};