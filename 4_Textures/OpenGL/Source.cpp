#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);



int main() {
	glfwInit();

	// Window configuration
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "Learning OpenGL", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	// Focus window
	glfwMakeContextCurrent(window);
	// Handle window rezising 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize glad and load all OpenGL function pointers.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	// Get available number of vertex attributes.
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	cout << "Maximum nr of vertex attribs supported: " << nrAttributes << endl; 


	// ---- SHADER PROGRAM ---- //
	Shader shaderProgram("shader.vs", "shader.fs");

	// ---- TRIANGLE VERTICES ---- //
	float vertices[] = {
		  0.0,  0.5, 0.0, 1.0f,  0.0f,  0.0f, // x, y, z, r, g, b
		 -0.5, -0.5, 0.0, 0.0f,  1.0f,  0.0f,
		  0.5, -0.5, 0.0, 0.0f,  0.0f,  1.0f,
	};

	// Texture coordinates for each vertex
	float texCoords[] = {
		0.5f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
	};

	// VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*
		- First parameter	-> specifies which vertex attribute to configure.
								Because the location of the position vertex attribute
								in the vertex shader was sepcified with layout (location = 0), we pass in 0.
		- Second parameter	-> specifies the size of the vertex attribute which is a vec3,
								so 3 values are passed.
		- Third parameter   -> type of the data, GL_FLOAT in this case because a vec* in GLSL
								consists of float values.
		- Fourth parameter	-> tell if the data should be normalized.
		- Fifth parameter   -> the stride. This tells the space between consecutive vertex attributes.
		- Sixth parameter   -> offset of where the position data begins in buffer.
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// Enable vertex attribute 0
	glEnableVertexAttribArray(0);

	// Enable vertex attribute 0
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind VAO
	glBindVertexArray(0);


	// App main loop
	while (!glfwWindowShouldClose(window)) {

		// ---INPUT---
		processInput(window);

		// ---RENDERING COMMANDS---
		// Assign a color to GL_COLOR_BUFFER_BIT and color the window
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Uniform

		// Use shader.h
		shaderProgram.use();

		// Drawing the triangle
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// ---CHECK & CALL EVENTS AND SWAP BUFFERS---
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	/*
		Callback function called every time the user
		resizes the window. The viewport doesn't get resized with the window 
		itself, so this is an overloaded function that takes a GLFWwindow and 
		the new width and height to set the new dimension of the glViewport.
	*/
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window) {
	// If escape is pressed while window is focused, tell glfw that window should close
	// glfwGetKey(window, GLFW_KEY) -> GLFW_PRESS if key is pressed else GLFW_RELEASE
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
