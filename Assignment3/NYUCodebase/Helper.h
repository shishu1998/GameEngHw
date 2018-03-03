#pragma once
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include "ShaderProgram.h"
#include "stb_image.h"
#include <vector>
GLuint LoadTexture(const char *filePath);
void DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing);