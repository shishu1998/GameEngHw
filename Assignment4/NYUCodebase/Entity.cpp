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
	if (!forward) modelMatrix.Scale(-1.0, 1.0, 0);

	Program.SetModelMatrix(modelMatrix);
	Program.SetViewMatrix(viewMatrix);

	sprite.Draw(&Program);
}

//Resets contact flags
void Entity::ResetContactFlags() {
	collidedLeft = collidedRight = collidedTop = collidedBottom = false;
}

//Checks collision with another entity
bool Entity::CollidesWith(const Entity& Other)
{
	return !(Position.y - size.y / 2 > Other.Position.y + Other.size.y / 2 || Position.y + size.y / 2 < Other.Position.y - Other.size.y / 2 || Position.x - size.x / 2 > Other.Position.x + Other.size.x / 2 || Position.x + size.x / 2 < Other.Position.x - Other.size.x / 2);
}

//Checks if the center of the left side is colliding with the tile to the left
void Entity::CollideLeft(int tileX) {
	float worldX = tileX * tileSize;
	if (Position.x - size.x / 2 < worldX + tileSize) {
		collidedLeft = true;
		acceleration.x = 0;
		velocity.x = 0;
		Position.x += (worldX + tileSize) - (Position.x - size.x / 2) + 0.01*tileSize;
	}
}

//Checks if the center of the right side is colliding with the tile to the right
void Entity::CollideRight(int tileX) {
	float worldX = tileX * tileSize;
	if (Position.x + size.x / 2 > worldX) {
		collidedRight = true;
		acceleration.x = 0;
		velocity.x = 0;
		Position.x -= (Position.x + size.x / 2 - worldX + 0.01*tileSize);
	}
}

//Checks if the center of the top side is colliding with the tile to the top
void Entity::CollideTop(int tileY) {
	float worldY = tileY * -tileSize;
	if (Position.y + size.y / 2 > worldY - tileSize) {
		collidedTop = true;
		acceleration.y = 0;
		velocity.y = 0;
		Position.y -= ((Position.y + size.y / 2) - (worldY - tileSize) + tileSize * 0.01);
	}
}

//Checks if the center of the bottom side is colliding with the tile to the bottom
void Entity::CollideBottom(int tileY) {
	float worldY = tileY * -tileSize;
	if (Position.y - size.y / 2 < worldY) {
		collidedBottom = true;
		acceleration.y = 0;
		velocity.y = 0;
		Position.y += (worldY - (Position.y - size.y / 2)) + tileSize * 0.01;
	}
}

//Checks if the current entity is colliding with tiles to any side
void Entity::CollidesWithTile(const std::vector<std::vector<unsigned int>>& mapData, std::unordered_set<int> solids)
{
	int gridX, gridY, gridLeft, gridRight, gridTop, gridBottom;
	worldToTileCoordinates(Position.x, Position.y, &gridX, &gridY);
	worldToTileCoordinates(Position.x - size.x / 2, Position.y - size.y / 2, &gridLeft, &gridBottom);
	worldToTileCoordinates(Position.x + size.x / 2, Position.y + size.y / 2, &gridRight, &gridTop);
	if (solids.find(mapData[gridTop][gridX]) != solids.end()) CollideTop(gridTop);
	if (solids.find(mapData[gridBottom][gridX]) != solids.end()) CollideBottom(gridBottom);
	if (solids.find(mapData[gridY][gridLeft]) != solids.end()) CollideLeft(gridLeft);
	if (solids.find(mapData[gridY][gridRight]) != solids.end()) CollideRight(gridRight);
}

//Updates the position of current entity
void Entity::Update(float elapsed)
{
	ResetContactFlags();
	velocity.x = lerp(velocity.x, 0.0f, elapsed * Friction_X);
	velocity.x += acceleration.x * elapsed;
	velocity.y += GRAVITY * elapsed;
	Position.y += velocity.y * elapsed;
	Position.x += velocity.x * elapsed;
}
