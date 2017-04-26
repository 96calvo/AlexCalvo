//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include "Shader2.h"

using namespace std;
#define PI 3.14159265
const GLint WIDTH = 800, HEIGHT = 600;
bool WIREFRAME = false;
int screenWithd, screenHeight;



void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


int main() {

	//initGLFW
	//TODO

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

	//TODO

	//set GLEW and inicializate
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		std::cout << "Error al inicializar glew" << std::endl;
		glfwTerminate();
		return NULL;
	}

	//TODO

	//set function when callback
	glfwSetKeyCallback(window, key_callback);
	//TODO

	int screenWithd, screenHeight;
	glfwGetFramebufferSize(window, &screenWithd, &screenHeight);
	glViewport(0, 0, screenWithd, screenHeight);

	//cargamos los shader
	Shader move("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");
	
	// Definir el buffer de vertices
	GLfloat VertexBufferObject[] = {

		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,

		0.0f,  0.0f, 1.0f,  // Top Right
		0.0f,  1.0f, 0.0f,	// Bottom Right
		0.0f,  1.0f, 0.0f, // Bottom Left
		0.0f,  0.0f, 1.0f // Top Left 
	};


	// Definir el EBO

	// Crear los VBO, VAO y EBO
	glClearColor(0.6f, 0.6f, 1.0f, 1.f);
	GLuint IndexBufferObject[]{
		0,1,2,
		0,2,3
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

	//Alocamos memoria suficiente para almacenar 4 grupos de 3 floats (segundo parámetro)
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferObject), VertexBufferObject, GL_STATIC_DRAW);
	//Alocamos ahora el EBO()
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexBufferObject), IndexBufferObject, GL_STATIC_DRAW);
	//Establecer las propiedades de los vertices

	//buffer de posiciones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//buffer de color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	GLfloat lastTime = 0;
	GLfloat angle = 0;
	GLfloat increment = 0;

	//bucle de dibujado
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		glClearColor(0.0f, 1.0f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Establecer el shader

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
		
		//GLint offset;
		//offset = (glGetUniformLocation(move.Program, "offset"));
		//glUniform1f(offset, cos(abs(glfwGetTime())));

		if (WIREFRAME) {

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		//pintar el VAO

		// bind index buffer if you want to render indexed data


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
}



