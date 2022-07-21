#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// GLSL (GL Shading Language)
/*
	- #version 460 core -> indicate that the OpenGL version is 4.6 and it's core.
	- layout (location = 0) in vec3 aPos -> get attribute 0 and put it in a 3D vector called aPos.
	- glPosition = vec4(...) -> predefined vec4 variable, since aPos is a vec3 and a vec4 is needed
								the 4th parameter (width) is set to 1.0f. This is the output of the
								vertex shader.
*/
const char* vertexShaderSource =
"#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"} \0";

/*
	out vec4 FragColor -> indicates that a vec4 named FragColor will be the output.
	FragColor = vec4(R, G, B, A) (all between 0.0 and 1.0)
*/
const char* fragmentShaderSource =
"#version 460 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
	// Load all OpenGL function pointers (NEED TO RESEARCH ABOUT THIS)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	// ---- VERTEX SHADER ----- // 
	unsigned int vertexShader; // it has an ID
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // This parameter is used because we are creating a vertex shader.

	// glShaderSource(shader object to compile, number of strings passed as source code, source code, IGNORE)
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check if shader compilation was successful
	int success;												// indicator of success of compilation
	char infoLog[512];											// storage for possible error messages
	//  glGetShaderiv(supposedly compiled shader, compilation status, variable for assigning the status to)
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); // Get the error log and assign it to infoLog
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << endl;
	}

	// ---- FRAGMENT SHADER ---- //
	/* This shader will calculate the color (in RGBA format) output of the pixels */
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Same as vertex shader													
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog); // Get the error log and assign it to infoLog
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
			infoLog << endl;
	}


	unsigned int shaderProgram;
	shaderProgram = glCreateProgram(); // returns an int ID reference

	// Attach the created files to the shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); // Link shaders

	// Check if the shaders were linked together
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" <<
			infoLog << endl;
	}

	// glLinkProgram creates a program object that can be activated.
	// Every shader and rendering after glUseProgram will use this program obj and it's shaders
	glUseProgram(shaderProgram);

	// Shaders that've been linked into the program object can be deleted
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// ---- TRIANGLE VERTICES ---- //
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	// ---- VERTEX ARRAY OBJECT AND VERTEX BUFFER OBJECT ---- //
	/*
		A VAO can be bound like a VBO and any subsequent vertex attribute calls
		from that point on will be stored inside the VAO.
		This has the advantage that when configuring vertex attribute objects
		we only have to make those calls once and whenever we want to draw the object,
		we can just bind the corresponding VAO, so we can switch between different
		vertex data and attribute configurations.

		************* NEED TO LEARN MORE ABOUT VBO AND VAO *************
	*/
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// 1- glBindVertexArray is used to bind the VAO.
	glBindVertexArray(VAO);

	// 2- copy vertices array in a buffer for OpenGL to use

	// Create a vertex buffer object (VBO) to store a large number of vertices in
	// the GPU's memory, sending data in large batches all at once (keeping it in memory
	// if there's enough memory left) insted of sending it one vertex at a time.
	// This is a OpenGL object and therefore has a unique buffer ID, generated with:
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// This binds the created buffer to the GL_ARRAY_BUFFER target i.e. tells OpenGL what's the buffer type.
	// OpenGL has many types of buffers. The buffer type of a vertex buffer is GL_ARRAY_BUFFER.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	/*
		glBufferData is specifically targetted to copy user-defined data into the currently bound buffer.
		The first arguemnt is the type of the buffer to copy the data into: the vertex buffer is currently
		bound to the GL_ARRAY_BUFFER target.
		The second argument is the size of the data in bytes that is going to be passed to the buffer.
		The third parameter is the actual data to send.

		The fourth parameter specifies how the GPU will manage the data:
		-- GL_STREAM_DRAW: data is set only once and used by the GPU at most a few times.
		-- GL_STATIC_DRAW: data is set only once and used many times.
		-- GL_DYNAMIC_DRAW: data is changed a lot and used many times.
	*/

	// 3- Set vertex attribute pointers
	/*
		The vertex buffer data consists of a lot of values tightly packed together.
		OpenGL needs to know how to interpret this vertex data before rendering.
		In this case the buffer data is 36 bytes long because each vertex
		has an x, y and z value, and for each of those numbers is stored in 4 bytes.
		So all the data for just one vertex is 12 bytes long, so we say that the
		stride of the buffer is 12.
		The first value in the data is at the beginning of the buffer and after the z value
		of this first vertex it's the x value of the second vertex and so on.
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*
		We need to tell OpenGL all this in glVertexAttribPointer:

		- First parameter	-> specifies which vertex attribute to configure.
								Because the location of the position vertex attribute
								in the vertex shader was sepcified with layout (location = 0),
								this sets the location of the vertex attribute to 0 and since
								we want to pass data to this vertex attribute, we pass in 0.
		- Second parameter	-> specifies the size of the vertex attribute which is a vec3,
								so 3 values are passed.
		- Third parameter   -> type of the data, GL_FLOAT in this case because a vec* in GLSL
								consists of float values.
		- Fourth parameter	-> tell if the data should be normalized.
		- Fifth parameter   -> the stride. This tells the space between consecutive vertex attributes.
								sizeof(float) = 4 so the stride is 12 as said before.
		- Sixth parameter   -> offset of where the position data begins in buffer. The weird cast is
								because it has a type of void*. The position data is at the start of the
								buffer, the value is 0.

		Each vertex attribute takes its data from memory managed by a VBO. Because there
		can be multiple VBOs, the VBO used its the one that's currently boud to GL_ARRAY_BUFFER
		when calling glVertexAttribPointer.
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// Now the vertex attribute must be enabled. We pass 0 because it's the vertex attribute location.
	glEnableVertexAttribArray(0);

	// We can unbind the VAO like this (must be done after finishing to use it):
	glBindVertexArray(0);

	// 4- draw object (should go in app loop)
	// glUseProgram(shaderProgram)
	// glBindVertexArray(VAO);

	// App main loop
	while (!glfwWindowShouldClose(window)) {

		// ---INPUT---
		processInput(window);

		// ---RENDERING COMMANDS---
		// Clear color buffer and fill it with a color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Drawing the triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// ---CHECK & CALL EVENTS AND SWAP BUFFERS---
		// Handle and poll I.O. and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
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
