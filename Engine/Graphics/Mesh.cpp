// Includes
//=========

#include "Mesh.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Logging/Logging.h>

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