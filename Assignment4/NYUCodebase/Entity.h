#pragma once
#include "ShaderProgram.h"
#include "helper.h"
#include "SheetSprite.h"

enum EntityType {Player, Enemy, Bullet, Life};
class Entity {
public:
	Entity();
	Entity(float x, float y, EntityType type, SheetSprite sprite);
	void Draw(ShaderProgram &Program);
	bool CollidesWith(const Entity& Other) const;
	void Move(float elapsed);
	float x;
	float y;
	float rotation = 0;
	int textureID;
	float width;
	float height;
	float velocity_x = 0.0f;
	float velocity_y = 0.0f;
	int health;
	EntityType type;
	SheetSprite sprite;
};