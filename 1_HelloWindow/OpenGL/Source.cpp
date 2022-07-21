#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
	glfwInit();

	/*
		glfwWindowHint allows to configure a specific setting.
		The first argument is the name of the setting, stored in
		a enum of all available options with prefix GLFW_...
		The second argument is the value to set to that option.

		All the options: https://www.glfw.org/docs/latest/window.html#window_hints
	*/

	// Sets the version of opengl that glfw will use, in this case it's 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfwCreateWindow(width, height, title, IGNORE, IGNORE) -> window handler (pointer) or NULL if failure
	GLFWwindow* window = glfwCreateWindow(800, 600, "Learning OpenGL", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	/*
	Function to link the resize action on a window (first argument)
	to a callback function that adjusts the viewport.
*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize glad
	// Load all OpenGL function pointers (NEED TO RESEARCH ABOUT THIS)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	/*
		glfwWindowShouldClose checks on each iteration if GLFW has been told to close.
		So if this is false, then the window should remain open.
	*/
	while (!glfwWindowShouldClose(window)) {

		// ---INPUT---
		processInput(window);

		// ---RENDERING COMMANDS---
		// Colors the window
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		/*
			 glClear clears the passed buffer of bits,
			 in this case is like clearing the screen
			 so the results of the previous frame don't overlap.
			 Can only pass GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT or GL_STENCIL_BUFFER_BIT as the parameter.

			 Also, whenever glClear is called to clear the color buffer, the buffer will be filled
			 with the color as configured by glClearColor
		*/
		glClear(GL_COLOR_BUFFER_BIT);

		// ---CHECK & CALL EVENTS AND SWAP BUFFERS---
		// Check for triggered events, update window state and call some function(s).
		glfwPollEvents();
		/*
			Swaps the color buffer (2D buffer containing color values for each pixel in window)
			used to render to during a specific render iteration and shows it as output on screen.
			
			It's called "swapBuffers" because there are actually two buffers, one at the front and
			one at the back. The front one contains the output image that's being displayed, while
			the rendering commands draw to the back buffer. When all the rendering commands are finished,
			the buffers are swapped, so the back buffer becomes the front one and viceversa.
			This way the image isn't shown if it hasn't finished rendering, removing all artifacts that could
			appear.
		*/
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
