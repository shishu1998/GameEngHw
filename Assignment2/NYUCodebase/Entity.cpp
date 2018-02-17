#include "Entity.h"



Entity::Entity(float x, float y, float width, float height): x(x), y(y), width(width), height(height) {}

void Entity::Move()
{
	x += velX;
	y += velY;
}

void Entity::Draw(ShaderProgram &Program) const
{
	glUseProgram(Program.programID);

	Matrix modelMatrix;
	Matrix projectionMatrix;
	Matrix viewMatrix;

	projectionMatrix.SetOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
	modelMatrix.Translate(x, y, 0);

	Program.SetModelMatrix(modelMatrix);
	Program.SetProjectionMatrix(projectionMatrix);
	Program.SetViewMatrix(viewMatrix);

	float vertices[] = { -width / 2, -height / 2, width / 2, -height / 2, width / 2, height / 2, -width / 2, -height / 2, width / 2, height / 2, -width / 2, height / 2 };
	glVertexAttribPointer(Program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(Program.positionAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(Program.positionAttribute);
}

bool Entity::Collision(const Entity & Other) const
{
	return !(y - height / 2 > Other.y + Other.height / 2 || y + height / 2 < Other.y - Other.height / 2 || x - width / 2 > Other.x + Other.width / 2 || x + width / 2 < Other.x - Other.width / 2);
}
