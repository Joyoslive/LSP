#include "pch.h"
#include "AssimpLoader.h"

AssimpLoader::AssimpLoader() :
	m_meshVertexCount(0),
	m_meshIndexCount(0)
{
}

AssimpLoader::~AssimpLoader()
{
}

EngineMeshData AssimpLoader::loadStaticModel(std::string filePath)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(
		filePath,
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_DropNormals		// Added 15/03/2021
	);

	if (scene == nullptr)
	{
		OutputDebugStringW(L"Assimp: File not found!");
		assert(false);
	}

	unsigned int totalVertexCount = 0;
	unsigned int totalSubsetCount = 0;
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		totalVertexCount += scene->mMeshes[i]->mNumVertices;
		++totalSubsetCount;
	}
	m_vertices.reserve(totalVertexCount);
	m_indices.reserve(totalVertexCount);
	m_subsets.reserve(totalSubsetCount);

	processNode(scene->mRootNode, scene);

	EngineMeshData data;
	data.indices = m_indices;
	data.vertices = m_vertices;
	data.subsetsInfo = m_subsets;

	m_indices.clear();
	m_vertices.clear();
	m_subsets.clear();
	m_meshVertexCount = 0;
	m_meshIndexCount = 0;
	return data;
}

void AssimpLoader::processNode(aiNode* node, const aiScene* scene)
{
	// For each mesh in the node, process it!
	for (int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
	}

	for (int i = 0; i < node->mNumChildren; ++i)
	{
		processNode(node->mChildren[i], scene);
	}

}

// Subset of Mesh
void AssimpLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
	for (int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vert = { };
		vert.pos.x = mesh->mVertices[i].x;
		vert.pos.y = mesh->mVertices[i].y;
		vert.pos.z = mesh->mVertices[i].z;

		vert.normal.x = mesh->mNormals[i].x;
		vert.normal.y = mesh->mNormals[i].y;
		vert.normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			vert.uv.x = mesh->mTextureCoords[0][i].x;
			vert.uv.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vert.uv.x = 0.0f;
			vert.uv.y = 0.0f;
		}

		m_vertices.push_back(vert);
	}

	unsigned int indicesThisMesh = 0;
	for (int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];

		for (int j = 0; j < face.mNumIndices; ++j)
		{
			m_indices.push_back(face.mIndices[j]);
			++indicesThisMesh;
		}

	}

	// Get material
	auto mtl = scene->mMaterials[mesh->mMaterialIndex];
	aiString diffPath, norPath;
	mtl->GetTexture(aiTextureType_DIFFUSE, 0, &diffPath);
	mtl->GetTexture(aiTextureType_HEIGHT, 0, &norPath);

	aiColor3D color(0.f, 0.f, 0.f);
	mtl->Get(AI_MATKEY_COLOR_DIFFUSE, color);

	// Subset data
	EngineMeshSubset subsetData = { };
	subsetData.diffuseFilePath = diffPath.C_Str();
	subsetData.color[0] = color[0];
	subsetData.color[1] = color[1];
	subsetData.color[2] = color[2];
	subsetData.normalFilePath = norPath.C_Str();

	subsetData.vertexCount = mesh->mNumVertices;
	subsetData.vertexStart = m_meshVertexCount;
	m_meshVertexCount += mesh->mNumVertices;

	subsetData.indexCount = indicesThisMesh;
	subsetData.indexStart = m_meshIndexCount;
	m_meshIndexCount += indicesThisMesh;

	m_subsets.push_back(subsetData);
}
