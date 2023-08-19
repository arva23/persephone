#ifndef GUTILS_HPP_INCLUDED
#define GUTILS_HPP_INCLUDED

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>

namespace gui {

	inline GLuint loadShader(GLenum _shaderType, const char* _fileName) {
		GLuint loadedShader = glCreateShader(_shaderType);

		if (loadedShader == 0) {
			std::cerr << "[glCreateShader] Error during the initialization of shader: " << _fileName << "!\n";
			return 0;
		}

		std::string shaderCode = "";

		std::ifstream shaderStream(_fileName);
		if (!shaderStream.is_open()) {
			std::cerr << "[std::ifstream] Error during the reading of " << _fileName << " shaderfile's source!\n";
			return 0;
		}

		std::string line = "";
		while (std::getline(shaderStream, line)) {
			shaderCode += line + "\n";
		}

		shaderStream.close();

		const char* sourcePointer = shaderCode.c_str();
		glShaderSource(loadedShader, 1, &sourcePointer, nullptr);

		glCompileShader(loadedShader);

		GLint result = GL_FALSE;
		int infoLogLength;

		glGetShaderiv(loadedShader, GL_COMPILE_STATUS, &result);
		glGetShaderiv(loadedShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (GL_FALSE == result) {
			std::vector<char> VertexShaderErrorMessage(infoLogLength);
			glGetShaderInfoLog(loadedShader, infoLogLength, nullptr, &VertexShaderErrorMessage[0]);

			std::cerr << "[glCompileShader] Shader compilation error in " << _fileName << ":\n" << &VertexShaderErrorMessage[0] << std::endl;
		}

		return loadedShader;
	}
}

#endif// GUTILS_HPP_INCLUDED