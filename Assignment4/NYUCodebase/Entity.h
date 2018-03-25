#pragma once
#include "ShaderProgram.h"
#include "helper.h"
#include "SheetSprite.h"
#include "Vector3.h"
#define Friction_X 0.2f
#define GRAVITY -0.1f

enum EntityType {Player, Enemy};
class Entity {
public:
	Entity();
	Entity(float x, float y, SheetSprite sprite, EntityType type, bool isStatic);
	void Render(ShaderProgram &Program, Matrix viewMatrix);
	void ResetContactFlags();
	bool CollidesWith(const Entity& Other);
	bool CollidesWithTile(int tileX, int tileY);
	void CollideWithX(int tileX);
	void CollideWithY(int tileY);
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