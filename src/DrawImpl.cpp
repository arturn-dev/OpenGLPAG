#include <glad/glad.h>

#include "DrawImpl.h"



ElementDraw::ElementDraw(GLsizei count)
	: count(count)
{
}

ElementDraw::~ElementDraw()
{
}

void ElementDraw::draw()
{
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

ArraysMultiDraw::ArraysMultiDraw(const std::vector<GLint>& firstVerticesInMeshes,
	const std::vector<GLsizei>& verticesCountsInMeshes)
		: firstVerticesInMeshes(firstVerticesInMeshes), verticesCountsInMeshes(verticesCountsInMeshes)
{
	
}

void ArraysMultiDraw::draw()
{
	glMultiDrawArrays(GL_TRIANGLE_STRIP, firstVerticesInMeshes.data(), verticesCountsInMeshes.data(), verticesCountsInMeshes.size());
}
