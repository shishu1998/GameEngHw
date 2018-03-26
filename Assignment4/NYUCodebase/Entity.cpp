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

void Entity::CollideLeft(int tileX) {
	float worldX = tileX * tileSize;
	if (Position.x - size.x / 2 < worldX + tileSize) {
		collidedLeft = true;
		acceleration.x = 0;
		velocity.x = 0;
		Position.x += (worldX + tileSize) - (Position.x - size.x / 2) + 0.01*tileSize;
	}
}

void Entity::CollideRight(int tileX) {
	float worldX = tileX * tileSize;
	if (Position.x + size.x / 2 > worldX) {
		collidedRight = true;
		acceleration.x = 0;
		velocity.x = 0;
		Position.x -= (Position.x + size.x / 2 - worldX + 0.01*tileSize);
	}
}

void Entity::CollideTop(int tileY) {
	float worldY = tileY * -tileSize;
	if (Position.y + size.y / 2 > worldY - tileSize) {
		collidedTop = true;
		acceleration.y = 0;
		velocity.y = 0;
		Position.y -= ((Position.y + size.y / 2) - (worldY - tileSize) + tileSize * 0.01);
	}
}
void Entity::CollideBottom(int tileY) {
	float worldY = tileY * -tileSize;
	if (Position.y - size.y / 2 < worldY) {
		collidedBottom = true;
		acceleration.y = 0;
		velocity.y = 0;
		Position.y += (worldY - (Position.y - size.y / 2)) + tileSize * 0.01;
	}
}

void Entity::CollidesWithTile(const std::vector<std::vector<unsigned int>>& mapData)
{
	int playerGridX, playerGridY, playerGridLeft, playerGridRight, playerGridTop, playerGridBottom;
	worldToTileCoordinates(Position.x, Position.y, &playerGridX, &playerGridY);
	worldToTileCoordinates(Position.x - size.x / 2, Position.y - size.y / 2, &playerGridLeft, &playerGridBottom);
	worldToTileCoordinates(Position.x + size.x / 2, Position.y + size.y / 2, &playerGridRight, &playerGridTop);
	if (mapData[playerGridTop][playerGridX] != 0) CollideTop(playerGridTop);
	if (mapData[playerGridBottom][playerGridX] != 0) CollideBottom(playerGridBottom);
	if (mapData[playerGridY][playerGridLeft] != 0) CollideLeft(playerGridLeft);
	if (mapData[playerGridY][playerGridRight] != 0) CollideRight(playerGridRight);
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
