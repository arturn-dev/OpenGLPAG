#pragma once

#include <string>

#include "Model.h"

class AssimpModelLoader
{
	static std::string _modelDirPath;
	static std::string _textureDirPath;

	static std::string getModelPath(const std::string& filename);
	static std::string getTexturePath(const std::string& filename);

	static MeshCollection processNode(aiNode* node, const aiScene* scene);
	static Mesh createMesh(aiMesh* mesh, const aiScene* scene);
	static TextureCollection getTexturesOfType(aiMaterial* material, aiTextureType textureType, OpenGLRender::Texture::Type targetTextureType);
	
public:
	static void setModelDirPath(const std::string& modelDirPath);
	static void setTextureDirPath(const std::string& textureDirPath);
	static Model loadModel(const std::string& modelFilename);
};