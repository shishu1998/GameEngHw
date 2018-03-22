#pragma once
#include "ShaderProgram.h"
#include "helper.h"
#include "SheetSprite.h"
#include "Vector3.h"

enum EntityType {Player, Enemy, Bullet, Life};
class Entity {
public:
	Entity();
	Entity(float x, float y, EntityType type, SheetSprite sprite);
	void Render(ShaderProgram &Program);
	bool CollidesWith(const Entity& Other);
	void Update(float elapsed);

	SheetSprite sprite;

	Vector3 Position;
	Vector3 size;
	Vector3 velocity;
	Vector3 acceleration;

	bool isStatic;
	EntityType entityType;
	
	bool collidedTop = false;
	bool collidedBottom = false;
	bool collidedLeft = false;
	bool collidedRight = false;
};