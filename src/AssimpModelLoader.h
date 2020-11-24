#pragma once

#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Model.h"

class AssimpModelLoader
{
	std::string modelDirPath;
	std::string textureDirPath;
	ShaderProgram shaderProgram;
	aiColor4D vertexColor;

	MeshCollection processNode(aiNode* node, const aiScene* scene);
	Mesh createMesh(aiMesh* mesh, const aiScene* scene);
	TextureCollection getTexturesOfType(aiMaterial* material, aiTextureType textureType, OpenGLRender::Texture::Type targetTextureType);
	
public:
	AssimpModelLoader(const std::string& modelDirPath, const std::string& textureDirPath,
	                  const ShaderProgram& shaderProgram);

	void setModelDirPath(const std::string& modelDirPath);
	void setTextureDirPath(const std::string& textureDirPath);
	void setShaderProgram(ShaderProgram shaderProgram);
	
	Model loadModel(const std::string& modelFilename, aiColor4D col = aiColor4D{0.0f});

	std::string getModelPath(const std::string& filename);
	std::string getTexturePath(const std::string& filename);
};