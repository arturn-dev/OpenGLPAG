#include "MengerCube.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

MengerCube::MengerCube(const MengerCubeFragmentsCache& mengerCubeFragments, unsigned int n, float edgeLength)
	: n(n), edgeLength(edgeLength)
{
	const float fragmentSize = edgeLength / pow(3.0f, static_cast<float>(n));

	int i = 1;
	for (auto&& fragmentCoords : mengerCubeFragments.getFragmentsCoords())
	{
		const auto* coordsPtr = fragmentCoords.data();
		fragmentsLocalCoords.push_back(glm::vec3(coordsPtr[0] * fragmentSize, coordsPtr[1] * fragmentSize, coordsPtr[2] * fragmentSize));

		// only for the n-levels of recursion (do not use entire fragments cache)
		if (i < static_cast<int>(pow(20, n)))
			i++;
		else
			break;
	}
}

MengerCube::~MengerCube()
{
}

ColMengerCube::ColMengerCube(const MengerCubeFragmentsCache& mengerCubeFragments, unsigned int n, float edgeLength, 
							 GLint modelMatLocation, GLuint aPos, GLuint aCol)
	: MengerCube(mengerCubeFragments, n > mengerCubeFragments.getN() ? mengerCubeFragments.getN() : n, edgeLength),
      ColCuboid(modelMatLocation, aPos, aCol, glm::vec3(edgeLength / pow(3.0f, static_cast<float>(n))))
{
	
}

ColMengerCube::~ColMengerCube()
{
}

void ColMengerCube::draw()
{
	const float threeToNth = pow(3.0f, static_cast<float>(n));
	const float fragmentSize = edgeLength / threeToNth;
	const glm::vec3 cubeBuildPivot = -glm::vec3(edgeLength / 2 - fragmentSize / 2);
	const glm::mat4 buildPivotMat = glm::translate(modelMat, cubeBuildPivot);

	glBindVertexArray(getVao());
	
	int i = 0;
	for (auto&& fragmentCoords : fragmentsLocalCoords)
	{
		glm::mat4 tmpModelMat = glm::translate(buildPivotMat, fragmentCoords);
		glUniformMatrix4fv(getModelMatLocation(), 1, GL_FALSE, glm::value_ptr(tmpModelMat));
		ColCuboid::internalDraw();
	}

	glBindVertexArray(0);
}

TexMengerCube::TexMengerCube(const MengerCubeFragmentsCache& mengerCubeFragments, unsigned n, float edgeLength,
	GLint modelMatLocation, const std::string& texturePath, GLuint aPos, GLuint aTex)
	: MengerCube(mengerCubeFragments, n > mengerCubeFragments.getN() ? mengerCubeFragments.getN() : n, edgeLength),
      TexCuboid(modelMatLocation, texturePath, aPos, aTex, glm::vec3(edgeLength / pow(3.0f, static_cast<float>(n))))
{
}

TexMengerCube::~TexMengerCube()
{
}

void TexMengerCube::draw()
{
	const float threeToNth = pow(3.0f, static_cast<float>(n));
	const float fragmentSize = edgeLength / threeToNth;
	const glm::vec3 cubeBuildPivot = -glm::vec3(edgeLength / 2 - fragmentSize / 2);
	const glm::mat4 buildPivotMat = glm::translate(modelMat, cubeBuildPivot);
	
	glBindVertexArray(getVao());
	
	int i = 0;
	for (auto&& fragmentCoords : fragmentsLocalCoords)
	{
		glm::mat4 tmpModelMat = glm::translate(buildPivotMat, fragmentCoords);
		glUniformMatrix4fv(getModelMatLocation(), 1, GL_FALSE, glm::value_ptr(tmpModelMat));
		TexCuboid::internalDraw();
	}

	glBindVertexArray(0);
}
