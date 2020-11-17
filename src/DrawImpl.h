#pragma once

#include <vector>

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
	~ElementDraw();
	
	void draw() override;
};

class ArraysMultiDraw : public DrawImpl
{
public:
	ArraysMultiDraw(const std::vector<GLint>& firstVerticesInMeshes, const std::vector<GLsizei>& verticesCountsInMeshes);
	// TODO: move class's attribs to private
	std::vector<GLint> firstVerticesInMeshes;
	std::vector<GLsizei> verticesCountsInMeshes;
	
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