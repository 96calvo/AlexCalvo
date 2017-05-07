#pragma once

#include <GLFW\glfw3.h>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

const GLint WIDTH = 800, HEIGHT = 600;

class Camara {
private:
	vec3 cameraPos;
	vec3 cameraFront;
	vec3 cameraUp;
	GLfloat Deltatime = glfwGetTime();
	GLfloat Lastframe;
	GLfloat LastMx = WIDTH / 2.0; 
	GLfloat LastMy = HEIGHT / 2.0;;
	GLfloat Sensitivity;
	GLboolean firstMouse=false;
	GLfloat PITCH = 0.0f;
	GLfloat YAW = -90.0f;
	GLfloat FOV;
public: 
	Camara(vec3 position, vec3 direction, GLfloat sensitivity, GLfloat FOV);
	void DoMovement(GLFWwindow * windowd);
	void MouseMove(GLFWwindow* window, double xpos, double ypos);
	void MouseScroll(GLFWwindow* window, double xScroll, double yScroll);
	mat4 LookAt();
	GLfloat GetFOV();


};
