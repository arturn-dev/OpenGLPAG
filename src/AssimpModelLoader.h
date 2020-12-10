#pragma once

#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Model.h"

template <typename T>
class AssimpModelLoader
{
	std::string modelDirPath;
	std::string textureDirPath;
	ShaderProgram shaderProgram;
	aiColor4D vertexColor;
	bool overwriteColor = false;

	std::vector<T> processNode(aiNode* node, const aiScene* scene);
	T createMesh(aiMesh* mesh, const aiScene* scene);
	void populateIndices(aiMesh* mesh, IndexCollection& indices);
	TextureCollection getTexturesOfType(aiMaterial* material, aiTextureType textureType, OpenGLRender::Texture::Type targetTextureType);

public:
	AssimpModelLoader(const std::string& modelDirPath, const std::string& textureDirPath);

	void setModelDirPath(const std::string& modelDirPath);
	void setTextureDirPath(const std::string& textureDirPath);
	void setShaderProgram(ShaderProgram shaderProgram);

	Model<T> loadModel(const std::string& modelFilename, const ShaderProgram& shaderProgram, aiColor4D col = aiColor4D{0.0f});

	std::string getModelPath(const std::string& filename);
	std::string getTexturePath(const std::string& filename);
};
