#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "stb_image.h"

#include <iostream>


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
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Focus window
	glfwMakeContextCurrent(window);
	// Handle window rezising 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize glad and load all OpenGL function pointers.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ---- SHADER PROGRAM ---- //
	Shader shaderProgram("shader.vs", "shader.fs");

	// ---- TRIANGLE VERTICES ---- //

	float vertices[] = {
		// xyz -> position, rgb -> color, st -> texture coords
		//   x,     y,   z,     s,    t,
		  0.5f,  0.5f, 0.0f, 1.0f, 1.0f,	// top right
		  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,	// bottom right
		 -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,	// bottom left
		 -0.5f,  0.5f, 0.0f, 0.0f, 1.0f		// top left
	};

	unsigned int indices[] = {
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture coordinate attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// ---- LOAD AND CREATE TEXTURE ---- //
	// Create texture objects
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Texture wrapping and filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load the first image.
	int width, height, nrChannels;

	// Flip image.
	stbi_set_flip_vertically_on_load(true);

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
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}


	// Good practice to free image from memory
	stbi_image_free(data);

	// Now activate, bind and configure second texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// Texture wrapping nad filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load the second image.
	data = stbi_load("./awesomeface.png", &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	// Tell OpenGL to which texture unit each shader sampler2D belongs to
	shaderProgram.use();
	shaderProgram.setInt("texture1", 0);
	shaderProgram.setInt("texture2", 1);

	// App main loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// Remember matrix combining order scaling->rotation->transformation

		//// First, transform is a 4x4 identity matrix
		//glm::mat4 transform = glm::mat4(1.0f);
		//// transform becomes a rotation matrix. Rotates a vector 90 degrees along the z-axis
		//transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		//// transform also becomes a matrix that scales a vector by 0.5 (multiplies all vertex position vector by 0.5)
		//transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));



		// First, transform is a 4x4 identity matrix
		glm::mat4 transform = glm::mat4(1.0f);
		// transform also becomes a rotation matrix. Rotates a vector over time along the z-axis
		transform = glm::rotate(transform, (float)glfwGetTime(), // time here is interpreted as radians
			glm::vec3(0.0, 0.0, 1.0));
		// transform becomes a matrix that translates a vector (adds the specified vector to each vertex position vector)
		transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));


		// Localize the transform uniform in the shader
		unsigned int transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
						   // location, # of matrices to send, transpose matrix?, matrix data
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		shaderProgram.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// de-allocate all resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
