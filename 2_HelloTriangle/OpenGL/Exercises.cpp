#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

unsigned int compileShader(unsigned int shaderType, const char* source);
unsigned int createshaderProgram1(const char* vsSource, const char* fsSource);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


// GLSL (GL Shading Language)

const char* vertexShaderSource =
"#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"} \0";

// Orange-ish
const char* fragmentShader1Source =
"#version 460 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"} \0";

// Yellow
const char* fragmentShader2Source =
"#version 460 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"	FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"} \0";

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

	// Initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}


	// ---- SHADER PROGRAM ---- //
	unsigned int shaderProgram1;
	shaderProgram1 = createshaderProgram1(vertexShaderSource, fragmentShader1Source);

	unsigned int shaderProgram2;
	shaderProgram2 = createshaderProgram1(vertexShaderSource, fragmentShader2Source);


	// ---- RECTANGLE VERTICES ---- //
	// Vertices for triangles
	float vertices1[] = {
		 -0.5,  0.5, 0.0,
		 -0.5, -0.5, 0.0,
		  0.5, -0.5, 0.0,
		  0.5,  0.5, 0.0,
	};

	// Indices for EBO
	int indices[] = {
		0, 1, 2,
		0, 2, 3,
		2, 4, 5
	};

	// Generate 2 Vertex Array Objects
	unsigned int VAOs[2];
	glGenVertexArrays(2, VAOs);

	// Generate 2 Vertex Buffer Objects
	unsigned int VBOs[2];
	glGenBuffers(2, VBOs);

	// Bind first VAO
	glBindVertexArray(VAOs[0]);

	// Bind first VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	// Configure attribute pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Element Buffer Object
	unsigned int EBO1;
	glGenBuffers(1, &EBO1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// ---- TRIANGLE VERTICES ---- //
	float vertices2[] = {
		  0.5, -0.5, 0.0,
		 0.75,  0.5, 0.0,
			1, -0.5, 0.0
	};

	// Bind second VAO
	glBindVertexArray(VAOs[1]);

	// Bind second VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind VAO2
	glBindVertexArray(0);

	// Main loop
	while (!glfwWindowShouldClose(window)) {

		// ---INPUT---
		processInput(window);

		// ---RENDERING COMMANDS---
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Drawing the triangle
		glUseProgram(shaderProgram1);
		glBindVertexArray(VAOs[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glUseProgram(shaderProgram2);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// ---CHECK & CALL EVENTS AND SWAP BUFFERS---
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

unsigned int compileShader(unsigned int shaderType, const char* source) {

	/* VERTEX SHADER: in this case will tell where to draw each vertex i.e. it's position */
	/* FRAGMENT SHADER: will calculate the color (in RGBA format) output of the pixels */

	unsigned int id; // it has an ID
	id = glCreateShader(shaderType); // GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...

	// glShaderSource(id of shader object to compile, number of strings passed as source code, source code, IGNORE)
	glShaderSource(id, 1, &source, NULL);
	glCompileShader(id);

	// Check if shader compilation was successful
	int success;												// indicator of success of compilation
	char infoLog[512];											// storage for possible error messages
	//  glGetShaderiv(supposedly compiled shader, compilation status, variable for assigning the status to)
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) { // !success
		glGetShaderInfoLog(id, 512, NULL, infoLog); // Get the error log and assign it to infoLog
		cout << "ERROR::SHADER::" << (shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" <<
			infoLog << endl;
		return 0; // id of 0 is interpreted as nothing
	}

	return id;
}

unsigned int createshaderProgram1(const char* vsSource, const char* fsSource) { // vs: vertexShader and fs: fragmentShader
	// ---- SHADER PROGRAM ---- //
	/*
		Shader program is the final linked version of multiple shaders combined.
		When linking shaders into a program it links the outputs of each shader
		to the inputs of the next shader.
	*/
	unsigned int program = glCreateProgram();

	unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
	unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);

	// Attach shaders to shader program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program); // Link shaders

	// Check if the shaders were linked together
	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) { // !success
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << endl;
		return 0; // id of 0 is interpreted as nothing
	}

	// glLinkProgram creates a program object that can be activated.
	// Every shader and rendering after glUseProgram will use this program obj and it's shaders
	glUseProgram(program);

	// Shaders that've been linked into the program object can be deleted
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	/*
		Callback function that will be called every time the user
		resizes the window. This must be done because the viewport
		doesn't get resized with the window itself, so this is an overloaded
		function that takes a GLFWwindow and the new width and height to set
		the new dimension of the glViewport.
	*/


	/*
		This tells OpenGL the size of the rendering window so it knows
		how we want to display the data and coordinates with respect to the window.
		The first two parameters set the location of the lower left corner of the window.
		The other two set the width and height of the rendering window in pixels.
	*/
	glViewport(0, 0, width, height);

}

void processInput(GLFWwindow* window) {
	// If escape is pressed while window is focused, tell glfw that window should close
	// glfwGetKey(window, GLFW_KEY) -> GLFW_PRESS if key is pressed else GLFW_RELEASE
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
