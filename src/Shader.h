#pragma once

#include <string>

#include <glad/glad.h>

class Shader
{
	GLuint shaderID;
	std::string fileName;
	
	std::string readShaderFile(const std::string& fileName);
	
public:
	Shader(std::string fileName, unsigned int type);

	void compileShader();
	void deleteShader();

	GLuint getShaderId() const;
};

