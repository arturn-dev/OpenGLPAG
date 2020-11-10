#include <glad/glad.h>

#include "DrawImpl.h"


DrawImpl::DrawImpl()
{
}

DrawImpl::~DrawImpl()
{
}

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
