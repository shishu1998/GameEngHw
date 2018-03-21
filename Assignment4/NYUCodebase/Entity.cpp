#include "Entity.h"

Entity::Entity() {}
Entity::Entity(float x, float y, EntityType type, SheetSprite sprite) : Position(x,y,0), 
size(sprite.width * sprite.size/ sprite.height, sprite.size, 0), entityType(type), sprite(sprite) {
}

void Entity::Render(ShaderProgram & Program)
{
	Matrix modelMatrix;
	Matrix projectionMatrix;
	Matrix viewMatrix;

	projectionMatrix.SetOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
	modelMatrix.Translate(Position.x, Position.y, 0);

	Program.SetModelMatrix(modelMatrix);
	Program.SetProjectionMatrix(projectionMatrix);
	Program.SetViewMatrix(viewMatrix);

	sprite.Draw(&Program);
}

bool Entity::CollidesWith(const Entity & Other) const
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
}
