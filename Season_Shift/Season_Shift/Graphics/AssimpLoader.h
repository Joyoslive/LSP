#pragma once

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <vector>
#include <string>

#include "Mesh.h"
#include "DX/DXDevice.h"



class AssimpLoader
{
private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<EngineMeshSubset> m_subsets;
	// Keep track of submesh offset in m_vertices and m_indices respectively
	unsigned int m_meshVertexCount;
	unsigned int m_meshIndexCount;

	void processNode(aiNode* node, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene);

public:
	AssimpLoader();
	~AssimpLoader();

	EngineMeshData loadStaticModel(std::string filePath);





};