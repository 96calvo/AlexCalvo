
#include "Camara.h"

Camara::Camara(vec3 position, vec3 direction, GLfloat sensitivity, GLfloat Fov) {
	cameraPos = position;
	cameraFront = direction;
	Sensitivity = sensitivity;
	FOV = Fov;	
	vec3 cameraDirection = normalize(cameraPos - cameraFront);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 cameraRight = normalize(cross(up, cameraDirection));
	cameraUp = cross(cameraDirection, cameraRight);
	
}


void Camara::DoMovement(GLFWwindow * window) {
	
	bool KeyW = false;
	bool KeyA = false;
	bool KeyS = false;
	bool KeyD = false;

	if (glfwGetKey(window, GLFW_KEY_W)) {
		KeyW = true;
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		KeyA = true;
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		KeyS = true;
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		KeyD = true;
	}
	if (KeyW) {
		cameraPos.z -= 0.1f;
		KeyW = false;
	}
	if (KeyA) {
		cameraPos.x -= 0.1f;
		KeyA = false;
	}
	if (KeyS) {
		cameraPos.z += 0.1f;
		KeyS = false;
	}
	if (KeyD) {
		cameraPos.x += 0.1f;
		KeyD = false;
	}
	
}
	


void Camara::MouseMove(GLFWwindow* window, double xpos, double ypos) {
	
	if (firstMouse) {
		LastMx = xpos;
		LastMy = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - LastMx;
	GLfloat yoffset = LastMy - ypos;
	LastMx = xpos;
	LastMy = ypos;


	xoffset *= Sensitivity;
	yoffset *= Sensitivity;

	YAW += xoffset;
	PITCH += yoffset;

	if (PITCH > 89.0f)
		PITCH = 89.0f;
	if (PITCH < -89.0f)
		PITCH = -89.0f;
	/*if (YAW > 89.0f)
		YAW = 89.0f;
	if (YAW < -89.0f)
		YAW = -89.0f;*/

	glm::vec3 front;
	front.x = cos(radians(YAW)) * cos(radians(PITCH));
	front.y = sin(radians(PITCH));
	front.z = sin(radians(YAW)) * cos(radians(PITCH));

	cameraFront = normalize(front);
	
}

void Camara::MouseScroll(GLFWwindow* window, double xScroll, double yScroll) {
	
	if (FOV >= 1.0f && FOV <= 100.0f)
		FOV -= yScroll;
	if (FOV <= 1.0f)
		FOV = 1.0f;
	if (FOV >= 45.0f)
		FOV = 45.0f;
	
}

mat4 Camara::LookAt()
{
	return lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}


GLfloat Camara::GetFOV()
{
	return FOV;
}
