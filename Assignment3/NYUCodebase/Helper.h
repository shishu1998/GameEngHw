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
#include "stb_image.h"
#include <vector>

GLuint LoadTexture(const char *filePath);
void DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing);
void DrawMessage(ShaderProgram& program, std::string upper, std::string lower);