#pragma once
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif
#include "ShaderProgram.h"
#include "SheetSprite.h"
#include "stb_image.h"
#include <vector>

GLuint LoadTexture(const char *filePath);
void DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing);
void DrawMessage(ShaderProgram& program, int TextureID, std::string text, float x, float y, float size, float space);
SheetSprite& createSheetSprite(unsigned int textureID, float x, float y, float width, float height, float size);
float lerp(float v0, float v1, float t);