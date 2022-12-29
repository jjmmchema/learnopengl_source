#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include <stb/stb_image.h>

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

	// Texture wrapping and filtering
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


	// ---- CUBE VERTICES ---- //

	float vertices[] = {
		// xyz -> position, st -> texture coords
		//   x,     y,   z,     s,    t,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
			   
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f
	};


	// VAO, VBO
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture coordinate attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// View matrix, move the scene a bit forward (-z axis) to see it
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	// Perspective projection matrix;
	glm::mat4 projection;			// fov, aspect ratio, near plane, far plane
	projection = glm::perspective(glm::radians(45.f), 800.0f / 600.0f, 0.1f, 100.0f);

	// Enable depth test (z-buffer or depth buffer)
	glEnable(GL_DEPTH_TEST);

	// Cube positions for drawing multiple cubes

	glm::vec3 cubePositions[] = {
		glm::vec3( 0.0f,  0.0f,   0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f,  -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3 (2.4f, -0.4f,  -3.5f),
		glm::vec3(-1.7f,  3.0f,  -7.5f),
		glm::vec3( 1.3f, -2.0f,  -2.5f),
		glm::vec3( 1.5f,  2.0f,  -2.5f),
		glm::vec3( 1.5f,  0.2f,  -1.5f),
		glm::vec3(-1.3f,  1.0f,  -1.5f)
	};


	// ----- CAMERA ----- //

	/*
		Needed to define a camera:
		a) Position in world space
		b) Camera direction
		c) A vector pointing to the right
		d) A vector pointing upwards from the camera
	*/

	// 3.0f in z-axis moves the camera backwards
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	
	// The camera points at the origin of the scene
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	/*
		The camera direction vector is given by subtracting the
		focus point (origin) from the camera position vector, i.e.
		cameraTarget - cameraPos,
		but since by convention the camera points towards the
		negative z-axis, the substraction order is switched:
		cameraPos - cameraTarget
	*/
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);


	/*
		The right vector represents the positive x-axis of the camera space.
		If cameraDirection is a vector with only a non-zero z-component, we
		can get this right vector by defining a unit vector in the +y direction
		and then apply the cross product between both.
		Remember that the cross product gives a new vector perpendicular to the plane
		defined by both vectors.
		Therefore, the cross product between the cameraDirection and the +y vector
		will result in a vector pointing at the +x or -x axis, depending of the
		order of the operands.
	*/
	glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(globalUp, cameraDirection));

	// And the cross product between the cameraDirection (a vector pointing in the +z camera axis)
	// and the cameraRight (a vector pointing in the +x camera axis) gives a new vector perpendicular to both
	// that will point directly upwards from the camera.

	// No glm::normalize because both vector are already normalized
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	
	/*
		Read LaTex notes for explanation about the lookAt matrix.
		In summary, lookAt matrix will be used to transform any vector
		to the camera's coordinate space by multiplying it with this matrix.
	*/
	//glm::mat4 view;
	//				// Camera position            // Camera target             // Vector pointing upwards from the camera
	//view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// ----- CAMERA ----- //

	// App main loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clears GL_DEPTH_BUFFER_BIT before each render iteration

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// Get location of needed matrix uniforms
		int projectionLoc = glGetUniformLocation(shaderProgram.ID, "projection");

		// Send matrices data to the respective uniforms
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		shaderProgram.use();
		glBindVertexArray(VAO);

		// ----- CAMERA POSITION ----- //

		const float radius = 10.0f;
		float elapsedTime = (float)glfwGetTime();
		float camX = sin(elapsedTime) * radius;
		float camZ = cos(elapsedTime) * radius;

		glm::mat4 view;
		view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shaderProgram.setMat4("view", GL_FALSE, view);

		// ----- CAMERA POSITION ----- //

		// Draw each cube by modyfing the model matrix
		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			// Send matrix data to the respective uniform
			shaderProgram.setMat4("model", GL_FALSE, model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

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
