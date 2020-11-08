#include <stdexcept>

#include "ShaderProgram.h"

std::string ShaderProgram::getInfoLog()
{
	const int maxInfoLogSize = 10000;
	GLchar infoLog[maxInfoLogSize];	
	glGetProgramInfoLog(programID, maxInfoLogSize, nullptr, infoLog);
	return std::string(infoLog);
}

ShaderProgram::ShaderProgram(): attribPos(-1U), attribCol(-1U), attribTex(-1U), attribNorm(-1U)
{
	programID = glCreateProgram();
	if (programID == NULL)
		throw std::logic_error("glCreateProgram() failed");
}

void ShaderProgram::setAttribPosByName(const std::string& attribPosName)
{
	attribPos = glGetAttribLocation(programID, attribPosName.c_str());
}

void ShaderProgram::setAttribColByName(const std::string& attribColName)
{
	attribCol = glGetAttribLocation(programID, attribColName.c_str());
}

void ShaderProgram::setAttribTexByName(const std::string& attribTexName)
{
	attribTex = glGetAttribLocation(programID, attribTexName.c_str());
}

void ShaderProgram::setAttribNormByName(const std::string& attribNormName)
{
	attribNorm = glGetAttribLocation(programID, attribNormName.c_str());
}

void ShaderProgram::attachShader(const Shader& shader)
{
	glAttachShader(programID, shader.getShaderId());
}

void ShaderProgram::makeProgram()
{
	glLinkProgram(programID);
	
	// Verification of linking
	GLint success;
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		throw std::logic_error(getInfoLog().insert(0, "[ShaderProgram] Error! Unsuccessful linking of shader program:\n"));
	}

	// Validaton of program
	glGetProgramiv(programID, GL_VALIDATE_STATUS, &success);
	if (!success)
	{
		throw std::logic_error(getInfoLog().insert(0, "[ShaderProgram] Error! Unsuccessful validation of shader program:\n"));
	}
}

void ShaderProgram::use()
{
	glUseProgram(programID);
}

void ShaderProgram::deleteProgram()
{
	glDeleteProgram(programID);
}

GLint ShaderProgram::getUniformLocation(const std::string& name)
{
	return glGetUniformLocation(programID, name.c_str());	
}

GLuint ShaderProgram::getProgramId() const
{
	return programID;
}

GLuint ShaderProgram::getAttribPos() const
{
	return attribPos;
}

GLuint ShaderProgram::getAttribCol() const
{
	return attribCol;
}

GLuint ShaderProgram::getAttribTex() const
{
	return attribTex;
}

GLuint ShaderProgram::getAttribNorm() const
{
	return attribNorm;
}
