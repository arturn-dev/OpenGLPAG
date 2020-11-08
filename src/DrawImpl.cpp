#include <glad/glad.h>

#include "DrawImpl.h"


DrawImpl::DrawImpl()
{
}

DrawImpl::~DrawImpl()
{
}

ElementDraw::ElementDraw(GLsizei count, const void* indices)
	: count(count), indices(indices)
{
}

ElementDraw::~ElementDraw()
{
}

void ElementDraw::draw()
{
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_BYTE, indices);
}
