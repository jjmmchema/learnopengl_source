#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "std_image.h"

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

	// ---- SHADER PROGRAM ---- //
	Shader shaderProgram("shader.vs", "shader.fs");

	// ---- TRIANGLE VERTICES ---- //
	float vertices[] = {
		// xyz -> position, rgb -> color, st -> texture coords
		//   x,     y,   z,     r,    g,   b,     s,    t,
		  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	};

	float indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	// VAO, VBO, EBO
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// ---- LOAD AND CREATE TEXTURE ---- //
	// Create texture object
	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture); // Bind texture

	// Texture wrapping on bound texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	// Texture filtering on bound texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load the image. width -> image width, height -> image height, nrChannels -> number of color channels
	int width, height, nrChannels;
	unsigned char* data = stbi_load("./container.jpg", &width, &height, &nrChannels, 0);

	if (data) {
		// Create texture
		/*
			- 1st param -> Texture target. This generates a texture to the currently bound
						   texture object at the target specified.
			- 2nd param -> Mipmap level for which we want to create a texture for.
			- 3rd param -> Type of format to store the texture.
			- 4th and 5th param -> width and height of the texture.
			- 6th param -> MUST be 0 for legacy.
			- 7th and 8th -> format and datatype of source image. The image is in RGB and
							 stbi_load stores it as chars(bytes).
			- 9th param -> Image data.
		*/
		// When glTextImage2D is called the texture image is attached to the bound texture object.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		// Generate mipmap
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "Failed to load texture" << endl;
	}


	// Good practice to free image from memory
	stbi_image_free(data);

	// App main loop
	while (!glfwWindowShouldClose(window)) {

		// ---INPUT---
		processInput(window);

		// ---RENDERING COMMANDS---
		// Assign a color to GL_COLOR_BUFFER_BIT and color the window
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Uniform

		// Bind texture
		glBindTexture(GL_TEXTURE_2D, texture);

		// Use shader.h
		shaderProgram.use();

		// Drawing the rectangle
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
