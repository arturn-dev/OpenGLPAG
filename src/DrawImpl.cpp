#include <glad/glad.h>

#include "DrawImpl.h"

ElementDraw::ElementDraw(GLsizei count)
	: count(count)
{
}

void ElementDraw::draw()
{
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

GLsizei ElementDraw::getCount() const
{
	return count;
}

InstancedElementDraw::InstancedElementDraw(GLsizei elementsCount, GLsizei instancesCount)
	: elementsCount(elementsCount),	instancesCount(instancesCount)
{
	instanceVbo = std::shared_ptr<GLuint>(new GLuint, [](auto p) { glDeleteBuffers(1, p); });
	glGenBuffers(1, instanceVbo.get());
}

void InstancedElementDraw::init(ShaderProgram shaderProgram, GLuint vaoId)
{
	glBindVertexArray(vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, *instanceVbo);
	
	const auto instanceMatAttribLocation = glGetAttribLocation(shaderProgram.getProgramId(), "instanceMat");
	const GLsizei vec4Size = sizeof(glm::vec4);
	for (int i = 0; i < 4; ++i)
	{
		glVertexAttribPointer(instanceMatAttribLocation + i, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<const void*>(i * vec4Size));
		glEnableVertexAttribArray(instanceMatAttribLocation + i);
		glVertexAttribDivisor(instanceMatAttribLocation + i, 1);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void InstancedElementDraw::setInstancesData(const std::vector<glm::mat4>& instancesMats)
{
	glBindBuffer(GL_ARRAY_BUFFER, *instanceVbo);
	glBufferData(GL_ARRAY_BUFFER, instancesMats.size() * sizeof(glm::mat4), instancesMats.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstancedElementDraw::draw()
{
	glDrawElementsInstanced(GL_TRIANGLES, elementsCount, GL_UNSIGNED_INT, nullptr, instancesCount);
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

ArraysDraw::ArraysDraw(GLenum mode, GLsizei count): mode(mode),
                                                    count(count)
{
}

void ArraysDraw::draw()
{
	glDrawArrays(mode, 0, count);
}
