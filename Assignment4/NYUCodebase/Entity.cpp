#include "Entity.h"

Entity::Entity() {}
Entity::Entity(float x, float y, SheetSprite sprite, EntityType type) : Position(x,y,0), 
size(sprite.width * sprite.size/ sprite.height, sprite.size, 0), sprite(sprite), entityType(type) {
}

void Entity::Render(ShaderProgram & Program)
{
	Matrix modelMatrix;
	Matrix viewMatrix;
	if (entityType == Player) viewMatrix.Translate(-Position.x, -Position.y, 0);
	modelMatrix.Translate(Position.x, Position.y, 0);

	Program.SetModelMatrix(modelMatrix);
	Program.SetViewMatrix(viewMatrix);

	sprite.Draw(&Program);
}

bool Entity::CollidesWith(const Entity& Other)
{
	return !(Position.y - size.y / 2 > Other.Position.y + Other.size.y / 2 || Position.y + size.y / 2 < Other.Position.y - Other.size.y / 2 || Position.x - size.x / 2 > Other.Position.x + Other.size.x / 2 || Position.x + size.x / 2 < Other.Position.x - Other.size.x / 2);
}

void Entity::Update(float elapsed)
{
	velocity.x = lerp(velocity.x, 0.0f, elapsed * acceleration.x);
	velocity.y = lerp(velocity.y, 0.0f, elapsed * acceleration.y);
	velocity.x += acceleration.x * elapsed;
	velocity.y += acceleration.y * elapsed;
	Position.y += velocity.y * elapsed;
	//collisionY();
	Position.x += velocity.x * elapsed;
	//collisionX();
	collidedLeft = collidedRight = collidedTop = collidedBottom = false;
}