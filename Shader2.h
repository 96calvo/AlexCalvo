#pragma once

//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
private:
	GLuint Program;
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void USE();
	~Shader();

};

