#pragma once
#include <unordered_set>
#include "ShaderProgram.h"
#include "helper.h"
#include "SheetSprite.h"
#include "Vector4.h"
#define Friction_X 0.2f
#define GRAVITY -0.2f

enum EntityType {Player, Enemy};
class Entity {
public:
	Entity();
	Entity(float x, float y, SheetSprite sprite, EntityType type, bool isStatic);
	void Render(ShaderProgram &Program, Matrix viewMatrix);
	void ResetContactFlags();
	bool CollidesWith(const Entity& Other);
	void CollideLeft(int tileX);
	void CollideRight(int tileX);
	void CollideTop(int tileY);
	void CollideBottom(int tileY);
	void CollidesWithTile(const std::vector<std::vector<unsigned int>>& mapData, std::unordered_set<int> solids);

	void Update(float elapsed);

	SheetSprite sprite;

	Vector4 Position;
	Vector4 size;
	Vector4 velocity;
	Vector4 acceleration;

	bool isStatic;
	EntityType entityType;
	
	bool collidedTop = false;
	bool collidedBottom = false;
	bool collidedLeft = false;
	bool collidedRight = false;
	bool forward = true;
};