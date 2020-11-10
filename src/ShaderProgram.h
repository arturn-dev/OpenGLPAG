#pragma once

#include "Shader.h"

class ShaderProgram
{
	GLuint programID;
	GLuint attribPos, attribCol, attribTex, attribNorm;
	// TODO: Add shader's locations of matrices

	std::string getInfoLog();
	
public:
	ShaderProgram();
	
	void attachShader(const Shader& shader);
	void makeProgram();
	void setAttribPosByName(const std::string& attribPosName);
	void setAttribColByName(const std::string& attribColName);
	void setAttribTexByName(const std::string& attribTexName);
	void setAttribNormByName(const std::string& attribNormName);
	void use() const;
	void deleteProgram();

	GLint getUniformLocation(const std::string& name) const;
	GLuint getProgramId() const;
	GLuint getAttribPos() const;
	GLuint getAttribCol() const;
	GLuint getAttribTex() const;
	GLuint getAttribNorm() const;
};