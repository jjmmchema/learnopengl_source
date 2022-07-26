#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {

public:
	// Shader program ID
	unsigned int ID;
	// Constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath) {
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFFULY_READ\n" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		ID = Shader::createShaderProgram(vShaderCode, fShaderCode);

	}

	// Use/Activate the shader
	void use() {
		// Every shader and rendering after glUseProgram will use this program obj and it's shaders
		glUseProgram(ID);
	}

	/*
		Functions below allow to pass a value into some uniform
		defined into the shader program instantiated.

		ID -> the shader program id given by glCreateProgram()
		name -> name of the uniform defined in any on the shaders' GLSL
		value -> value to asign into uniform

		Note that only one value can be passed, since all calls to glUniform1i
		specify that 1 integer will be passed (it's a one not the letter l).

		Also, name.c_str() is called because that name is
		specified in the shader code and GLSL compilation implicitly adds a null character (\0)
		at the end of a string as it's done in C.

	*/
	void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}


private:
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
		char infoLog[1024];											// storage for possible error messages
		//  glGetShaderiv(supposedly compiled shader, compilation status, variable for assigning the status to)
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) { // !success
			glGetShaderInfoLog(id, 1024, NULL, infoLog); // Get the error log and assign it to infoLog
			std::cout << "ERROR::SHADER::" << (shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" <<
				infoLog << std::endl;
			return 0; // id of 0 is interpreted as nothing
		}

		return id;
	}

	unsigned int createShaderProgram(const char* vsSource, const char* fsSource) { // vs: vertexShader and fs: fragmentShader
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
		char infoLog[1024];
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (success == GL_FALSE) { // !success
			glGetProgramInfoLog(program, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
			return 0; // id of 0 is interpreted as nothing
		}

		// Shaders that've been linked into the program object can be deleted
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return program;
	}

};

#endif