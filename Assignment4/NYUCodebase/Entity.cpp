#include "Entity.h"

Entity::Entity() {}
Entity::Entity(float x, float y, EntityType type, SheetSprite sprite) : x(x), y(y), 
width(sprite.width * sprite.size/ sprite.height), height(sprite.size), type(type), sprite(sprite) {
	type == Player ? health = 3 : health = 1;
}

void Entity::Draw(ShaderProgram & Program)
{
	if (health > 0) {
		Matrix modelMatrix;
		Matrix projectionMatrix;
		Matrix viewMatrix;

		projectionMatrix.SetOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
		modelMatrix.Translate(x, y, 0);
		modelMatrix.Rotate(rotation);

		Program.SetModelMatrix(modelMatrix);
		Program.SetProjectionMatrix(projectionMatrix);
		Program.SetViewMatrix(viewMatrix);

		sprite.Draw(&Program);
	}
}

bool Entity::CollidesWith(const Entity & Other) const
{
	return !(y - height / 2 > Other.y + Other.height / 2 || y + height / 2 < Other.y - Other.height / 2 || x - width / 2 > Other.x + Other.width / 2 || x + width / 2 < Other.x - Other.width / 2);
}

void Entity::Move(float elapsed)
{
	if (velocity_x < 0 && x - width / 2 > -3.55 || velocity_x > 0 && x + width / 2 < 3.55) {
		x += elapsed * velocity_x;
	}
	y += elapsed * velocity_y;
}
