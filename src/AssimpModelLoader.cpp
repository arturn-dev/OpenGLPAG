#include <assimp/postprocess.h>
#include "AssimpModelLoader.h"

#include <stdexcept>


void AssimpModelLoader::setModelDirPath(const std::string& modelDirPath)
{
	this->modelDirPath = modelDirPath;
}

void AssimpModelLoader::setTextureDirPath(const std::string& textureDirPath)
{
	this->textureDirPath = textureDirPath;
}

void AssimpModelLoader::setShaderProgram(ShaderProgram shaderProgram)
{
	this->shaderProgram = shaderProgram;
}

std::string AssimpModelLoader::getModelPath(const std::string& filename)
{
	return modelDirPath + "\\" + filename;
}

std::string AssimpModelLoader::getTexturePath(const std::string& filename)
{
	return textureDirPath + "\\" + filename;
}

MeshCollection AssimpModelLoader::processNode(aiNode* node, const aiScene* scene)
{
	MeshCollection meshes;
	
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		auto* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(createMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		MeshCollection&& tmp = processNode(node->mChildren[i], scene);
		meshes.insert(meshes.end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
	}

	return meshes;
}

Mesh AssimpModelLoader::createMesh(aiMesh* mesh, const aiScene* scene)
{
	VertexCollection vertices;
	IndexCollection indices;
	TextureCollection textures;

	// Populate vertex collection.
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		aiVector2D texCoords;
		if (mesh->mTextureCoords[0])
		{
			texCoords = aiVector2D(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}

		vertices.emplace_back(mesh->mVertices[i], mesh->mNormals[i], texCoords);
	}
	
	// Populate index collection.
	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		TextureCollection ret = getTexturesOfType(material, aiTextureType_DIFFUSE, OpenGLRender::Texture::TexDiff);
		textures.insert(textures.end(), ret.begin(), ret.end());
		ret = getTexturesOfType(material, aiTextureType_SPECULAR, OpenGLRender::Texture::TexSpec);
		textures.insert(textures.end(), ret.begin(), ret.end());
	}

	return Mesh(vertices, indices, textures, shaderProgram);
}

TextureCollection AssimpModelLoader::getTexturesOfType(aiMaterial* material, aiTextureType textureType,
	OpenGLRender::Texture::Type targetTextureType)
{
	TextureCollection textures;
	
	for (int i = 0; i < material->GetTextureCount(textureType); ++i)
	{
		aiString aistr;
		material->GetTexture(textureType, i, &aistr);
		textures.push_back({targetTextureType, getTexturePath(aistr.C_Str())});
	}

	return textures;
}

AssimpModelLoader::AssimpModelLoader(const std::string& modelDirPath, const std::string& textureDirPath,
                                     const ShaderProgram& shaderProgram): modelDirPath(modelDirPath),
                                                                          textureDirPath(textureDirPath),
                                                                          shaderProgram(shaderProgram)
{
}

Model AssimpModelLoader::loadModel(const std::string& modelFilename)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(getModelPath(modelFilename), aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		throw std::logic_error("[Assimp] " + std::string(importer.GetErrorString()));
	}

	return Model(processNode(scene->mRootNode, scene));
}
