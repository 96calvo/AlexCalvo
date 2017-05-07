//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Shader2.h"
#include "Camara.h"
#include "SOIL.h"


using namespace std;
using namespace glm;


//const GLint WIDTH = 800, HEIGHT = 600;
int screenWithd, screenHeight;

bool KeyUp = false;
bool KeyDown = false;
bool KeyLeft = false;
bool KeyRight = false;
bool Key1 = false;
bool Key2 = false;
bool KeyW = false;
bool KeyA = false;
bool KeyS = false;
bool KeyD = false;

vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
GLfloat sensitivity = 0.05;
GLfloat Fov = 45.0;




void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void DoMovement(GLFWwindow* window);
void UseMouse(GLFWwindow* window, double xpos, double ypos);
void UseScroll(GLFWwindow* window, double xoffset, double yoffset);

Camara newcam(cameraPos, cameraFront, sensitivity, Fov);


int main() {

	//initGLFW


	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//create a window
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);

	//comprobar que GLFW estaactivo
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Primera ventana", nullptr, nullptr);
	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	//set GLEW and inicializate
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		std::cout << "Error al inicializar glew" << std::endl;
		glfwTerminate();
		return NULL;
	}


	glEnable(GL_DEPTH_TEST);


	glfwGetFramebufferSize(window, &screenWithd, &screenHeight);
	
	glfwSetKeyCallback(window, key_callback);

	glfwSetScrollCallback(window, UseScroll);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	
	//Shader text("./src/textureVertex.vertexshader", "./src/textureFragment.fragmentshader");
	Shader coord("./src/coordVertex.vertexshader", "./src/coordFragment.fragmentshader");
	GLfloat VertexBufferCube[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	vec3 CubesPositionBuffer[] = {
		vec3(0.0f ,  0.0f,  0.0f),
		vec3(2.0f ,  5.0f, -15.0f),
		vec3(-1.5f, -2.2f, -2.5f),
		vec3(-3.8f, -2.0f, -12.3f),
		vec3(2.4f , -0.4f, -3.5f),
		vec3(-1.7f,  3.0f, -7.5f),
		vec3(1.3f , -2.0f, -2.5f),
		vec3(1.5f ,  2.0f, -2.5f),
		vec3(1.5f ,  0.2f, -1.5f),
		vec3(-1.3f,  1.0f, -1.5f)
	};



	GLuint VBO;
	//GLuint EBO;
	GLuint VAO;

	
	//reservar memoria para el VAO y VBO
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VAO);

	

	//Declarar el VBO y el EBO

	//Enlazar el buffer con openGL
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//Texture
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("./src/texture.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//
	int width2, height2;
	unsigned char* image2 = SOIL_load_image("./src/crash_bandicoot.png", &width2, &height2, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 1);


	mat4 projection;
	
	GLfloat opac = 0.0f;
	
	//Alocamos memoria suficiente para almacenar 4 grupos de 3 floats (segundo parámetro)
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferCube), VertexBufferCube, GL_STATIC_DRAW);

	//Establecer las propiedades de los vertices

	//buffer de posiciones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//buffer textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	//bucle de dibujado
	GLfloat anglex;
	GLfloat angley;

	mat4 view;

	GLint modelLoc;
	GLint viewLoc;
	GLint projLoc;
	/*
	vec3 cameraPos; //= vec3(0.0f, 0.0f, 3.0f);
	vec3 cameraTarget = vec3(0.0f, 0.0f, -1.0f);

	vec3 cameraDirection = normalize(cameraPos - cameraTarget);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 cameraRight = normalize(cross(up, cameraDirection));
	vec3 cameraUp = cross(cameraDirection, cameraRight);
	*/
	GLfloat radio = 8.0f;
	GLfloat X = 0.0f;
	GLfloat Z = 3.0f;

	GLuint i;


	
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		DoMovement(window);
		glfwSetCursorPosCallback(window, UseMouse);
		projection = perspective(newcam.GetFOV(), (GLfloat)screenWithd / (GLfloat)screenHeight, 0.1f, 100.0f);
		glClearColor(0.0f, 1.0f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Establecer el shader
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(coord.Program, "Texture"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(coord.Program, "Texture2"), 1);

		glUniform1f(glGetUniformLocation(coord.Program, "opac"), opac);


		if (Key1) {

			opac = 0.0f;
			
			glUniform1f(glGetUniformLocation(coord.Program, "opac"), opac);
			Key1 = false;
		}

		if (Key2) {
			opac = 1.0f;

			glUniform1f(glGetUniformLocation(coord.Program, "opac"), opac);
			Key2 = false;
		}


		if (KeyLeft) {
			angley = angley - 5.0f;
			KeyLeft = false;
		}
		if (KeyRight) {
			angley = angley + 5.0f;
			KeyRight = false;
		}
		if (KeyUp) {
			anglex = anglex - 5.0f;
			KeyUp = false;
		}
		if (KeyDown) {
			anglex = anglex + 5.0f;
			KeyDown = false;
		}

		coord.USE();
		/*
		mat4 transform;
		transform = scale(transform, vec3(0.5f, -0.5f, 0.0));
		transform = translate(transform, vec3(0.5f,0.5f,0.0));
		transform = rotate(transform, angle , vec3(0.0f,0.0f,1.0f));

		GLint transformlocation = glGetUniformLocation(coord.Program, "transform");

		glUniformMatrix4fv(transformlocation, 1, GL_FALSE , value_ptr(transform));
		*/


		//draw texture

		/*
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		*/
		mat4 model;
		model = rotate(model, anglex , vec3(1.0f,0.0f,0.0f));
		model = rotate(model, angley , vec3(0.0f, 1.0f, 0.0f));
		view = translate(view, vec3(0.0f, 0.0f, -3.0f));
		
		//X = sin(glfwGetTime()*3)*radio;
		//Z = cos(glfwGetTime()*3)*radio;
		/*
		 if (KeyW) {
			 Z -= 0.1f;
			 KeyW = false;
		 }
		 if (KeyA) {
			 X -= 0.1f;
			 KeyA = false;
		 }
		 if (KeyS) {
			 Z += 0.1f;
			 KeyS = false;
		 }
		 if (KeyD) {
			 X += 0.1f;
			 KeyD = false;
		 }
		 cameraPos = vec3(X, 0.0f, Z);
		 */
		 //view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);//lookAt(vec3(X, 0.0f, Z), vec3(0.0f, 0.0f, 0.0f),vec3(0.0f, 1.0f, 0.0f));
		view = newcam.LookAt();
		modelLoc = glGetUniformLocation(coord.Program, "model");
		viewLoc = glGetUniformLocation(coord.Program, "view");
		projLoc = glGetUniformLocation(coord.Program, "projection");
		glUniformMatrix4fv(modelLoc, 1,GL_FALSE,value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

		
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		for (i = 1; i < 10; ++i)
		{
			mat4 model2;
			model2 = translate(model2, CubesPositionBuffer[i]);
			model2 = rotate(model2, (GLfloat)glfwGetTime() * 3.0f, glm::vec3(1.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model2));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		glBindVertexArray(0);

		glfwSwapBuffers(window);

	}

	// liberar la memoria de los VAO, EBO y VBO

	// Terminate GLFW, clearing any resources allocated by GLFW.

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	glfwDestroyWindow(window);
	glfwTerminate();
	std::exit(EXIT_SUCCESS);
}

void DoMovement(GLFWwindow* window) {
	newcam.DoMovement(window);
	/*
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
	*/
}
//
//bool firstMouse = false;
/*
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
GLfloat YAW = -90.0f;
GLfloat PITCH = 0.0f;
*/

void UseMouse(GLFWwindow* window, double xpos, double ypos) {

	newcam.MouseMove(window, xpos, ypos);

	/*
	if (firstMouse){
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	
	
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;


	xoffset *= sensitivity;
	yoffset *= sensitivity;
	
	YAW += xoffset;
	PITCH += yoffset;

	if (PITCH > 89.0f)
		PITCH = 89.0f;
	if (PITCH < -89.0f)
		PITCH = -89.0f;

	glm::vec3 front;
	front.x = cos(radians(YAW)) * cos(radians(PITCH));
	front.y = sin(radians(PITCH));
	front.z = sin(radians(YAW)) * cos(radians(PITCH));
	
	cameraFront = normalize(front);
	
	*/
}


void UseScroll(GLFWwindow* window, double xoffset, double yoffset){
	
	newcam.MouseScroll(window, xoffset, yoffset);
	/*
	if (Fov >= 1.0f && Fov <= 45.0f)
		Fov -= yoffset;
	if (Fov <= 1.0f)
		Fov = 1.0f;
	if (Fov >= 45.0f)
		Fov = 45.0f;
	*/
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

	//Cuando pulsamos la tecla ESC se cierra la aplicacion
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	//Teclas Flechas
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		KeyUp = true;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		KeyDown = true;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		KeyLeft = true;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		KeyRight = true;
	}

	//Teclas Numericas
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		Key1 = true;
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		Key2 = true;
	}

}
