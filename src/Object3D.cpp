#include "Object3D.h"
#include "stb_image.h"
#include "stdexcept"

Object3D::Object3D(GLint modelMatLocation)
	: modelMatLocation(modelMatLocation)
{
}

Object3D::~Object3D()
{
}

GLuint Object3D::getVao() const
{
	return vao;
}

GLint Object3D::getModelMatLocation() const
{
	return modelMatLocation;
}

ColObject3D::ColObject3D(GLint modelMatLocation)
	: Object3D(modelMatLocation)
{
	
}

ColObject3D::~ColObject3D()
{
	deleteBuffers();
}

void ColObject3D::init(GLuint aPos, GLuint aCol)
{
	initBuffers();
	
	glVertexAttribPointer(aPos, ColVert::POS_COMP_COUNT, GL_FLOAT, GL_FALSE, ColVert::VERT_SIZE, nullptr);
	glEnableVertexAttribArray(aPos);
	glVertexAttribPointer(aCol, ColVert::COL_COMP_COUNT, GL_FLOAT, GL_FALSE, ColVert::VERT_SIZE, 
						  reinterpret_cast<const GLvoid*>(ColVert::POS_SIZE));
	glEnableVertexAttribArray(aCol);
}

void ColObject3D::initBuffers()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	std::vector<ColVert> verts;
	createVertexArray(verts);

	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(ColVert), verts.data(), GL_STATIC_DRAW);
}

void ColObject3D::deleteBuffers()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void TexObject3D::initTexture()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height, channelsCount;
	stbi_set_flip_vertically_on_load(true);
	stbi_uc* textureData = stbi_load(texturePath.c_str(), &width, &height, &channelsCount, 0);
	if (textureData == nullptr)
	{
		throw std::logic_error("Error loading texture.");
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(textureData);
}

void TexObject3D::init(GLuint aPos, GLuint aTex)
{
	initBuffers();

	glVertexAttribPointer(aPos, TexVert::POS_COMP_COUNT, GL_FLOAT, GL_FALSE, TexVert::VERT_SIZE, nullptr);
	glEnableVertexAttribArray(aPos);
	glVertexAttribPointer(aTex, TexVert::TEX_COMP_COUNT, GL_FLOAT, GL_FALSE, TexVert::VERT_SIZE,
	                      reinterpret_cast<const GLvoid*>(TexVert::POS_SIZE));
	glEnableVertexAttribArray(aTex);

	initTexture();
}

void TexObject3D::initBuffers()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	std::vector<TexVert> verts;
	createVertexArray(verts);

	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(TexVert), verts.data(), GL_STATIC_DRAW);
}

void TexObject3D::deleteBuffers()
{
	glDeleteTextures(1, &texture);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

TexObject3D::TexObject3D(GLint modelMatLocation, const std::string& texturePath)
	: Object3D(modelMatLocation), texturePath(texturePath)
{
}

TexObject3D::~TexObject3D()
{
	deleteBuffers();
}
