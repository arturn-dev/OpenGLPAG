#pragma once

#include "Shader.h"
#include "glm/vec3.hpp"

class ShaderProgram
{
	GLuint programID;
	GLuint attribPos, attribCol, attribTex, attribNorm;

	std::string getInfoLog();
	
public:
	ShaderProgram();
	
	void attachShader(const Shader& shader);
	void makeProgram();	
	void use() const;
	void deleteProgram();

	GLint getUniformLocation(const std::string& name) const;
	void setUniformVec3(const std::string& name, glm::vec3 vec) const;
	void setUniformMat4(const std::string& name, glm::mat4 mat) const;
	
	void setAttribPosByName(const std::string& attribPosName);
	void setAttribColByName(const std::string& attribColName);
	void setAttribTexByName(const std::string& attribTexName);
	void setAttribNormByName(const std::string& attribNormName);
	
	GLuint getProgramId() const;
	GLuint getAttribPos() const;
	GLuint getAttribCol() const;
	GLuint getAttribTex() const;
	GLuint getAttribNorm() const;
};
