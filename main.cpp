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

#include "SOIL.h"

using namespace std;
using namespace glm;

#define PI 3.14159265
const GLint WIDTH = 800, HEIGHT = 600;
bool WIREFRAME = false;
int screenWithd, screenHeight;

bool KeyUp = false;
bool KeyDown = false;
bool KeyLeft = false;
bool KeyRight = false;


void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


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

	//set function when callback
	

	//int screenWithd, screenHeight;
	glfwGetFramebufferSize(window, &screenWithd, &screenHeight);
	//glViewport(0, 0, screenWithd, screenHeight);
	glfwSetKeyCallback(window, key_callback);
	//cargamos los shader

	//Shader move("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");
	Shader text("./src/textureVertex.vertexshader", "./src/textureFragment.fragmentshader");

	// Definir el buffer de vertices
	GLfloat vertex[] = {

		// Positions          // Colors           // Texture Coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
	};


	// Definir el EBO

	// Crear los VBO, VAO y EBO
	
	GLuint IndexBufferObject[]{
		3,0,2,
		0,1,2
	};

	glfwGetTime();

	GLuint VBO;
	GLuint EBO;
	GLuint VAO;



	//reservar memoria para el VAO, VBO y EBO
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VAO);

	

	//Declarar el VBO y el EBO

	//Enlazar el buffer con openGL
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

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
	unsigned char* image2 = SOIL_load_image("./src/crash_bandicoo.png", &width2, &height2, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 1);

	GLfloat opac = 0.2f;
	
	//Alocamos memoria suficiente para almacenar 4 grupos de 3 floats (segundo parámetro)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	//Alocamos ahora el EBO()
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex), IndexBufferObject, GL_STATIC_DRAW);
	//Establecer las propiedades de los vertices

	//buffer de posiciones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//buffer de color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//buffer textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	/*
	GLfloat lastTime = 0;
	GLfloat angle = 0;
	GLfloat increment = 0;
	*/
	//bucle de dibujado
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		glClearColor(0.0f, 1.0f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Establecer el shader
		/*
		move.USE();
		
		GLint variableShader = glGetUniformLocation(move.Program, "offset");

		

		GLdouble currentTime = glfwGetTime(); 
		GLfloat deltatime = GLfloat(currentTime - lastTime);

		//Angulo

		if (deltatime > 0.04)
		{
			increment = abs(sin(angle / 180 * PI));

			//Pasamos el contenido al shader

			glUniform3f(variableShader, increment*0.5f, 0.0f, 0.0f);
			lastTime = currentTime;
			angle++;

		}

		//Comprobar que encuentra la variable Uniform del shader
		
		if (glGetUniformLocation(move.Program, "offset") == -1) {
		cout << "Error al localizar la variable Uniform" << endl;
		glfwTerminate();
		}
		*/
		//GLint offset;
		//offset = (glGetUniformLocation(move.Program, "offset"));
		//glUniform1f(offset, cos(abs(glfwGetTime())));

		if (WIREFRAME) {

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		text.USE();
		GLfloat angle;
		if (KeyLeft) {
			angle = angle + 5.0f;
			KeyLeft = false;
		}
		if (KeyRight) {
			angle = angle - 5.0f;
			KeyRight = false;
		}

		mat4 transform;
		transform = scale(transform, vec3(0.5f, -0.5f, 0.0));
		transform = translate(transform, vec3(0.5f,0.5f,0.0));
		transform = rotate(transform, angle , vec3(0.0f,0.0f,1.0f));

		GLint transformlocation = glGetUniformLocation(text.Program, "transform");


		glUniformMatrix4fv(transformlocation, 1, GL_FALSE , value_ptr(transform));
		//draw texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(text.Program, "Texture"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(text.Program, "Texture2"), 1);

		glUniform1f(glGetUniformLocation(text.Program, "opac"), opac);

		
		if (KeyUp) {

			if (opac <= 0) {
				opac = 0;
			}
			else {
				opac = opac - 0.1;
			}
			glUniform1f(glGetUniformLocation(text.Program, "opac"), opac);
			KeyUp = false;
		}
		if (KeyDown) {

			if (opac >= 1) {
				opac = 1;
			}
			else {
				opac = opac + 0.1;

			}
			glUniform1f(glGetUniformLocation(text.Program, "opac"), opac);
			KeyDown = false;
		}
		


		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);

	}

	// liberar la memoria de los VAO, EBO y VBO

	// Terminate GLFW, clearing any resources allocated by GLFW.

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwDestroyWindow(window);
	glfwTerminate();
	std::exit(EXIT_SUCCESS);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//Cuando pulsamos la tecla ESC se cierra la aplicacion
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	//Cuando apretamos la tecla W se cambia a modo Wireframe
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		WIREFRAME = !WIREFRAME;
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
}
