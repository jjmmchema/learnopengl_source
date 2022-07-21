#version 460 core
out vec4 FragColor;

in vec3 ourColor;

// out vec4 FragColor;
//uniform vec4 ourColor;

void main() {
	// FragColor = ourColor
	FragColor = vec4(ourColor, 1.0f);
};