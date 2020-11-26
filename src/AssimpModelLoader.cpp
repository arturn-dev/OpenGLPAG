#include <assimp/postprocess.h>
#include "AssimpModelLoader.h"

#include <stdexcept>

template <typename T>
void AssimpModelLoader<T>::setModelDirPath(const std::string& modelDirPath)
{
	this->modelDirPath = modelDirPath;
}

template <typename T>
void AssimpModelLoader<T>::setTextureDirPath(const std::string& textureDirPath)
{
	this->textureDirPath = textureDirPath;
}

template <typename T>
void AssimpModelLoader<T>::setShaderProgram(ShaderProgram shaderProgram)
{
	this->shaderProgram = shaderProgram;
}

template <typename T>
std::string AssimpModelLoader<T>::getModelPath(const std::string& filename)
{
	return modelDirPath + "\\" + filename;
}

template <typename T>
std::string AssimpModelLoader<T>::getTexturePath(const std::string& filename)
{
	return textureDirPath + "\\" + filename;
}

template <typename T>
std::vector<T> AssimpModelLoader<T>::processNode(aiNode* node, const aiScene* scene)
{
	std::vector<T> meshes;
	
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		auto* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(createMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		std::vector<T>&& tmp = processNode(node->mChildren[i], scene);
		meshes.insert(meshes.end(), std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()));
	}

	return meshes;
}

template <>
TexMesh AssimpModelLoader<TexMesh>::createMesh(aiMesh* mesh, const aiScene* scene)
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

		vertices.emplace_back(mesh->mVertices[i], mesh->mNormals[i], texCoords, vertexColor);
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

	return TexMesh(vertices, indices, shaderProgram, textures);
}

template <>
Mesh<ColVert> AssimpModelLoader<Mesh<ColVert>>::createMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<ColVert> vertices;
	IndexCollection indices;

	// Populate vertex collection.
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		vertices.emplace_back(mesh->mVertices[i], vertexColor);
	}
	
	// Populate index collection.
	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
	}

	return Mesh<ColVert>(vertices, indices, shaderProgram);
}

template <typename T>
TextureCollection AssimpModelLoader<T>::getTexturesOfType(aiMaterial* material, aiTextureType textureType,
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

template <typename T>
AssimpModelLoader<T>::AssimpModelLoader(const std::string& modelDirPath, const std::string& textureDirPath)
	: modelDirPath(modelDirPath), textureDirPath(textureDirPath)
{
}

template <typename T>
Model<T> AssimpModelLoader<T>::loadModel(const std::string& modelFilename, const ShaderProgram& shaderProgram, aiColor4D col)
{
	vertexColor = col;
	this->shaderProgram = shaderProgram;
		
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(getModelPath(modelFilename), aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		throw std::logic_error("[Assimp] " + std::string(importer.GetErrorString()));
	}
	
	return Model<T>(processNode(scene->mRootNode, scene));
}

template AssimpModelLoader<TexMesh>;
template AssimpModelLoader<Mesh<ColVert>>;
//template AssimpModelLoader<Mesh<TexVert>>;