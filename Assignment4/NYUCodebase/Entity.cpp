#include "Entity.h"

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

void Entity::Update(float elapsed)
{
	ResetContactFlags();
	velocity.x = lerp(velocity.x, 0.0f, elapsed * Friction_X);
	velocity.y = lerp(velocity.y, 0.0f, elapsed * GRAVITY);
	velocity.x += acceleration.x * elapsed;
	velocity.y += acceleration.y * elapsed;
	Position.y += velocity.y * elapsed;
	//collisionY();
	Position.x += velocity.x * elapsed;
	//collisionX();
}
