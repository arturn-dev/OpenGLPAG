#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "ShaderProgram.h"

class DrawImpl
{
public:	
	virtual void draw() = 0;
};

class ElementDraw : public DrawImpl
{
	GLsizei count;
	
public:
	ElementDraw(GLsizei count);
	
	void draw() override;
	GLsizei getCount() const;
};

class InstancedElementDraw : public DrawImpl
{
	std::shared_ptr<GLuint> instanceVbo;
	GLsizei elementsCount;
	GLsizei instancesCount;
	
public:
	InstancedElementDraw(GLsizei elementsCount, GLsizei instancesCount);
	
	void init(ShaderProgram shaderProgram, GLuint vaoId);
	void setInstancesData(const std::vector<glm::mat4>& instancesMats);
	void draw() override;
};

class ArraysMultiDraw : public DrawImpl
{
	std::vector<GLint> firstVerticesInMeshes;
	std::vector<GLsizei> verticesCountsInMeshes;
	
public:
	ArraysMultiDraw(const std::vector<GLint>& firstVerticesInMeshes, const std::vector<GLsizei>& verticesCountsInMeshes);
	
	void draw() override;
};

class ArraysDraw : public DrawImpl
{
	GLenum mode;
	GLsizei count;

public:
	ArraysDraw(GLenum mode, GLsizei count);

	void draw() override;
};