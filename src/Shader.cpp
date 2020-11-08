#include <fstream>
#include <sstream>

#include "Shader.h"

std::string Shader::readShaderFile(const std::string& fileName)
{
	std::ifstream f(fileName);
	std::stringstream buffer;
	buffer << f.rdbuf();

	return buffer.str();
}

Shader::Shader(std::string fileName, unsigned int type)
	: fileName(fileName)
{
	shaderID = glCreateShader(type);
	if (shaderID == NULL) 
		throw std::logic_error("glCreateShader() failed for type: " + std::to_string(type));

}

void Shader::compileShader()
{
	const std::string shaderCodeStr = readShaderFile(fileName);
	
	const char* shaderCodeStrPtr = shaderCodeStr.c_str();
	glShaderSource(shaderID, 1, static_cast<const GLchar**>(&shaderCodeStrPtr), nullptr);
	glCompileShader(shaderID);

	// Verify compilation
	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		const int maxInfoLogSize = 10000;
		GLchar infoLog[maxInfoLogSize];

		glGetShaderInfoLog(shaderID, maxInfoLogSize, NULL, infoLog);
		std::string infoLogStr = std::string(infoLog);
		infoLogStr.insert(0, "[Shader " + fileName + "] Error! Unsuccessful shader compilation:\n");

		throw std::logic_error(infoLogStr);
	}
}

void Shader::deleteShader()
{
	glDeleteShader(shaderID);
}

GLuint Shader::getShaderId() const
{
	return shaderID;
}
