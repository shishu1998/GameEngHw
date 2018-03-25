#include "Entity.h"
#include "Helper.h"

Entity::Entity() {}
Entity::Entity(float x, float y, SheetSprite sprite, EntityType type, bool isStatic) : Position(x,y,0), 
size(sprite.width * sprite.size/ sprite.height, sprite.size, 0), sprite(sprite), entityType(type), isStatic(isStatic) {
}

void Entity::Render(ShaderProgram & Program, Matrix viewMatrix)
{
	Matrix modelMatrix;
	modelMatrix.Translate(Position.x, Position.y, 0);

	Program.SetModelMatrix(modelMatrix);
	Program.SetViewMatrix(viewMatrix);

	sprite.Draw(&Program);
}

void Entity::ResetContactFlags() {
	collidedLeft = collidedRight = collidedTop = collidedBottom = false;
}

bool Entity::CollidesWith(const Entity& Other)
{
	return !(Position.y - size.y / 2 > Other.Position.y + Other.size.y / 2 || Position.y + size.y / 2 < Other.Position.y - Other.size.y / 2 || Position.x - size.x / 2 > Other.Position.x + Other.size.x / 2 || Position.x + size.x / 2 < Other.Position.x - Other.size.x / 2);
}

void Entity::CollideWithX(int tileX) {
	float worldX = tileX * tileSize;
	if (Position.x - size.x / 2 < worldX + tileSize) { 
		collidedLeft = true;
	}
	if (Position.x + size.x / 2 > worldX) { 
		collidedRight = true;
	}
}

void Entity::CollideWithY(int tileY) {
	float worldY = tileY * -tileSize;
	if (Position.y - size.y / 2 < worldY) { 
		collidedBottom = true;
		//Position.y += (worldY - (Position.y - size.y / 2)) + tileSize * 0.01;
	}
	if (Position.y + size.y / 2 > worldY - tileSize) { 
		collidedTop = true; 
		//Position.y -= ((Position.y + size.y / 2) - (worldY - tileSize) + tileSize * 0.01);
	}
}

bool Entity::CollidesWithTile(int tileX, int tileY)
{
	CollideWithX(tileX);
	CollideWithY(tileY);
	return collidedBottom || collidedTop || collidedLeft || collidedRight;
}

void Entity::Update(float elapsed)
{
	ResetContactFlags();
	velocity.x = lerp(velocity.x, 0.0f, elapsed * Friction_X);
	velocity.x += acceleration.x * elapsed;
	velocity.y += GRAVITY * elapsed;
	Position.y += velocity.y * elapsed;
	//collisionY();
	Position.x += velocity.x * elapsed;
	//collisionX();
}
