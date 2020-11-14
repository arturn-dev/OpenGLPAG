#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>

#include "TMat.h"

struct ColVert
{
	glm::vec3 pos;
	glm::vec4 col;

	static const unsigned int POS_COMP_COUNT = 3;
	static const unsigned int COL_COMP_COUNT = 4;
	static const auto VERT_COMP_COUNT = POS_COMP_COUNT + COL_COMP_COUNT;
	static const auto POS_SIZE = POS_COMP_COUNT * sizeof(GLfloat);
	static const auto COL_SIZE = COL_COMP_COUNT * sizeof(GLfloat);
	static const auto VERT_SIZE = POS_SIZE + COL_SIZE;

	ColVert(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
		: pos{x, y, z}, col{r, g, b, a}
	{}
};

struct TexVert
{
	glm::vec3 pos;
	glm::vec2 tex;

	static const unsigned int POS_COMP_COUNT = 3;
	static const unsigned int TEX_COMP_COUNT = 2;
	static const auto VERT_COMP_COUNT = POS_COMP_COUNT + TEX_COMP_COUNT;
	static const auto POS_SIZE = POS_COMP_COUNT * sizeof(GLfloat);
	static const auto TEX_SIZE = TEX_COMP_COUNT * sizeof(GLfloat);
	static const auto VERT_SIZE = POS_SIZE + TEX_SIZE;

	TexVert(GLfloat x, GLfloat y, GLfloat z, GLfloat s, GLfloat t)
		: pos{x, y, z}, tex{s, t}
	{}
};

class Object3D // TODO: Remove all OpenGL related functions and variables. Use the OpenGLRender class instead.
{
public:
	TMat modelMat;

protected:
	GLuint vao, vbo;
	GLint modelMatLocation;
	
	virtual void initBuffers() = 0;
	virtual void deleteBuffers() = 0;

public:
	Object3D() {}
	Object3D(GLint modelMatLocation);
	Object3D(const TMat& modelMat) : modelMat(modelMat) {}
	virtual ~Object3D();
	
	virtual void draw() = 0;

	GLuint getVao() const;
	GLint getModelMatLocation() const;
};

class ColObject3D : public Object3D
{
protected:
	void init(GLuint aPos, GLuint aCol);
	void initBuffers() override;
	void deleteBuffers() override;
	virtual void createVertexArray(std::vector<ColVert>& verts) = 0;
	
public:
	ColObject3D(GLint modelMatLocation);
	virtual ~ColObject3D();
	
	void draw() = 0;	
};

class TexObject3D : public Object3D
{
	std::string texturePath;

	void initTexture();
	
protected:
	unsigned int texture;
	
	void init(GLuint aPos, GLuint aTex);
	void initBuffers() override;
	void deleteBuffers() override;
	virtual void createVertexArray(std::vector<TexVert>& verts) = 0;
	
public:
	TexObject3D(GLint modelMatLocation, const std::string& texturePath);
	virtual ~TexObject3D();
	
	void draw() = 0;	
};