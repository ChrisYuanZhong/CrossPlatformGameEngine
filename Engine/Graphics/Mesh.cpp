// Includes
//=========

#include "Mesh.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Logging/Logging.h>
#include <iostream>
#include <fstream>

eae6320::cResult eae6320::Graphics::Mesh::LoadFromFile(Mesh*& o_mesh, const char* const meshPath)
{
	auto result = Results::Success;

	VertexFormats::sVertex_mesh* vertexData = nullptr;
	uint16_t* indexData = nullptr;
	unsigned int vertexCount;
	unsigned int indexCount;

	// Read from binary file

	// Open binary file
	std::ifstream infile(meshPath, std::ifstream::binary);
	if (!infile)
	{
		result = Results::Failure;
		EAE6320_ASSERTF(false, "Couldn't open mesh file");
		Logging::OutputError("Failed to open mesh file");
		return result;
	}

	// Read vertex count
	infile.read(reinterpret_cast<char*>(&vertexCount), sizeof(unsigned int));
	// Read vertex data
	vertexData = new VertexFormats::sVertex_mesh[vertexCount];
	infile.read(reinterpret_cast<char*>(vertexData), sizeof(VertexFormats::sVertex_mesh) * vertexCount);
	// Read index count
	infile.read(reinterpret_cast<char*>(&indexCount), sizeof(unsigned int));
	// Read index data
	indexData = new uint16_t[indexCount];
	infile.read(reinterpret_cast<char*>(indexData), sizeof(uint16_t) * indexCount);

	// Close binary file
	infile.close();

	// Use scope guard to make sure that the allocated memory is freed
	eae6320::cScopeGuard scopeGuard_vertexData([&vertexData, &indexData]
		{
			if (vertexData)
			{
				delete[] vertexData;
				vertexData = nullptr;
			}
			if (indexData)
			{
				delete[] indexData;
				indexData = nullptr;
			}
		});

	// Load mesh
	if (!(result = Load(o_mesh, vertexData, indexData, vertexCount, indexCount)))
	{
		EAE6320_ASSERTF(false, "Couldn't load mesh");
		return result;
	}

	return result;
}

eae6320::cResult eae6320::Graphics::Mesh::Load(Mesh*& o_mesh, VertexFormats::sVertex_mesh* i_vertexData, uint16_t* i_indexData, const unsigned int i_vertexCount, const unsigned int i_indexCount)
{
	auto result = Results::Success;

	Mesh* newMesh = nullptr;
	cScopeGuard cScopeGuard([&o_mesh, &result, &newMesh]
		{
			if (result)
			{
				EAE6320_ASSERT(newMesh != nullptr);
				o_mesh = newMesh;
			}
			else
			{
				if (newMesh)
				{
					newMesh->DecrementReferenceCount();
					newMesh = nullptr;
				}
				o_mesh = nullptr;
			}
		});

	// Allocate a new mesh
	{
		newMesh = new (std::nothrow) Mesh();
		if (!newMesh)
		{
			result = Results::OutOfMemory;
			EAE6320_ASSERTF(false, "Couldn't allocate memory for the mesh");
			Logging::OutputError("Failed to allocate memory for the mesh");
			return result;
		}
	}

	// Initialize the new mesh
	{
		if (!(result = newMesh->InitializeGeometry(i_vertexData, i_indexData, i_vertexCount, i_indexCount)))
		{
			EAE6320_ASSERTF(false, "Initialization of new mesh failed");
			return result;
		}
	}

	return result;
}

eae6320::Graphics::Mesh::~Mesh()
{
	cResult result = Results::Success;
	CleanUp(result);
	EAE6320_ASSERT(result);
}