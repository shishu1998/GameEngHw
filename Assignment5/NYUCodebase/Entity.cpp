#include "Entity.h"
#include "Helper.h"
#include "SatCollision.h"

Entity::Entity() : textured(false) {}
Entity::Entity(float x, float y, float width, float height, bool isStatic) : Position(x, y, 0), size(width, height, 0), isStatic(isStatic),textured(false) {
	matrix.Translate(Position.x, Position.y, 0);
}
Entity::Entity(float x, float y, SheetSprite sprite, EntityType type, bool isStatic) : Position(x,y,0), 
size(sprite.width * sprite.size/ sprite.height, sprite.size, 0), sprite(sprite), entityType(type), isStatic(isStatic), textured(true) {
	matrix.Translate(Position.x, Position.y, 0);
}

void Entity::UntexturedDraw(ShaderProgram & Program) {
	glUseProgram(Program.programID);
	float vertices[] = {
		-0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f,
		0.5f, 0.5f,
		-0.5f, -0.5f,
		0.5f, -0.5f };

	glVertexAttribPointer(Program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(Program.positionAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(Program.positionAttribute);
}

void Entity::Render(ShaderProgram & Program, Matrix viewMatrix)
{
	Matrix modelMatrix = matrix;
	if (!forward) modelMatrix.Scale(-1.0, 1.0, 0);

	Program.SetModelMatrix(modelMatrix);
	Program.SetViewMatrix(viewMatrix);

	textured ? sprite.Draw(&Program) : UntexturedDraw(Program);
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
		float leftPen = (worldX + tileSize) - (Position.x - size.x / 2) + 0.01*tileSize;
		Position.x += leftPen;
	}
}

//Checks if the center of the right side is colliding with the tile to the right
void Entity::CollideRight(int tileX) {
	float worldX = tileX * tileSize;
	if (Position.x + size.x / 2 > worldX) {
		collidedRight = true;
		acceleration.x = 0;
		velocity.x = 0;
		float rightPen = (Position.x + size.x / 2 - worldX + 0.01*tileSize);
		Position.x -= rightPen;
	}
}

//Checks if the center of the top side is colliding with the tile to the top
void Entity::CollideTop(int tileY) {
	float worldY = tileY * -tileSize;
	if (Position.y + size.y / 2 > worldY - tileSize) {
		collidedTop = true;
		acceleration.y = 0;
		velocity.y = 0;
		float topPen = ((Position.y + size.y / 2) - (worldY - tileSize) + tileSize * 0.01);
		Position.y -= topPen;
	}
}

//Checks if the center of the bottom side is colliding with the tile to the bottom
void Entity::CollideBottom(int tileY) {
	float worldY = tileY * -tileSize;
	if (Position.y - size.y / 2 < worldY) {
		collidedBottom = true;
		acceleration.y = 0;
		velocity.y = 0;
		float botPen = (worldY - (Position.y - size.y / 2)) + tileSize * 0.01;
		Position.y += botPen;
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

void Entity::remakeMatrix() {
	matrix.Identity();
	matrix.Translate(Position.x, Position.y, Position.z);
	matrix.Rotate(Rotation);
	matrix.Scale(size.x, size.y, size.z);
}

//Updates the position of current entity
void Entity::Update(float elapsed)
{
	ResetContactFlags();
	if (!isStatic) {
		velocity.x = lerp(velocity.x, 0.0f, elapsed * Friction_X);
		velocity.x += acceleration.x * elapsed;
		velocity.y += GRAVITY * elapsed;
		float displacementX = velocity.x * elapsed;
		float displacementY = velocity.y * elapsed;
		Position.y += displacementY;
		Position.x += displacementX;
	}
	remakeMatrix();
}

void Entity::Rotate(float angle) {
	Rotation += angle;
}

//Gets the corners of the entity
std::vector<std::pair<float, float>> Entity::getCorners() const {
	std::vector<std::pair<float, float>> vertices = std::vector<std::pair<float, float>>();
	Vector4 topLeft = matrix * Vector4(-0.5, 0.5, 0);
	Vector4 topRight = matrix * Vector4(0.5, 0.5, 0);
	Vector4 botLeft = matrix * Vector4(-0.5, -0.5, 0);
	Vector4 botRight = matrix * Vector4(0.5, -0.5, 0);
	vertices.emplace_back(topLeft.x, topLeft.y);
	vertices.emplace_back(topRight.x, topRight.y);
	vertices.emplace_back(botRight.x, botRight.y);
	vertices.emplace_back(botLeft.x, botLeft.y);
	return vertices;
}

//SAT Collision code
bool Entity::SATCollidesWith(Entity& Other) {
	std::pair<float, float> penetration;
	bool collided = CheckSATCollision(getCorners(), Other.getCorners(), penetration);
	if (Other.isStatic) {
		Position.x += penetration.first;
		Position.y += penetration.second;
	}
	else {
		Position.x += (penetration.first * 0.5f);
		Position.y += (penetration.second * 0.5f);
		Other.Position.x -= (penetration.first * 0.5f);
		Other.Position.y -= (penetration.second * 0.5f);
	}
	return collided;
}