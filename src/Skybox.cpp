#include <vector>

#include "Skybox.h"
#include "DrawImpl.h"
#include "Vertex.h"

Skybox::Skybox(const std::string& texturesDirPath, ShaderProgram shaderProgram)
	: openGlRender(shaderProgram, new ArraysDraw(GL_TRIANGLES, 36))
{
	std::vector<PosVert> skyboxVertices {
	    // positions          
	    PosVert{{-1.0f,  1.0f, -1.0f}},
	    PosVert{{-1.0f, -1.0f, -1.0f}},
	    PosVert{{ 1.0f, -1.0f, -1.0f}},
	    PosVert{{ 1.0f, -1.0f, -1.0f}},
	    PosVert{{ 1.0f,  1.0f, -1.0f}},
	    PosVert{{-1.0f,  1.0f, -1.0f}},

	    PosVert{{-1.0f, -1.0f,  1.0f}},
	    PosVert{{-1.0f, -1.0f, -1.0f}},
	    PosVert{{-1.0f,  1.0f, -1.0f}},
	    PosVert{{-1.0f,  1.0f, -1.0f}},
	    PosVert{{-1.0f,  1.0f,  1.0f}},
	    PosVert{{-1.0f, -1.0f,  1.0f}},

	    PosVert{{1.0f, -1.0f, -1.0f}},
	    PosVert{{1.0f, -1.0f,  1.0f}},
	    PosVert{{1.0f,  1.0f,  1.0f}},
	    PosVert{{1.0f,  1.0f,  1.0f}},
	    PosVert{{1.0f,  1.0f, -1.0f}},
	    PosVert{{1.0f, -1.0f, -1.0f}},

	    PosVert{{-1.0f, -1.0f,  1.0f}},
	    PosVert{{-1.0f,  1.0f,  1.0f}},
	    PosVert{{ 1.0f,  1.0f,  1.0f}},
	    PosVert{{ 1.0f,  1.0f,  1.0f}},
	    PosVert{{ 1.0f, -1.0f,  1.0f}},
	    PosVert{{-1.0f, -1.0f,  1.0f}},

	    PosVert{{-1.0f,  1.0f, -1.0f}},
	    PosVert{{ 1.0f,  1.0f, -1.0f}},
	    PosVert{{ 1.0f,  1.0f,  1.0f}},
	    PosVert{{ 1.0f,  1.0f,  1.0f}},
	    PosVert{{-1.0f,  1.0f,  1.0f}},
	    PosVert{{-1.0f,  1.0f, -1.0f}},

	    PosVert{{-1.0f, -1.0f, -1.0f}},
	    PosVert{{-1.0f, -1.0f,  1.0f}},
	    PosVert{{ 1.0f, -1.0f, -1.0f}},
	    PosVert{{ 1.0f, -1.0f, -1.0f}},
	    PosVert{{-1.0f, -1.0f,  1.0f}},
	    PosVert{{ 1.0f, -1.0f,  1.0f}}
	};

	openGlRender.setBufferData(skyboxVertices);
	openGlRender.addTexture(OpenGLRender::Texture{OpenGLRender::Texture::TexCubemap, texturesDirPath});
}

void Skybox::draw()
{
	glDepthMask(GL_FALSE);
	openGlRender.draw();
	glDepthMask(GL_TRUE);
}
