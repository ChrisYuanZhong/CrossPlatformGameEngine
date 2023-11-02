/*
	This class builds meshes
*/

#ifndef EAE6320_CMESHBUILDER_H
#define EAE6320_CMESHBUILDER_H

// Includes
//=========

#include <Tools/AssetBuildLibrary/iBuilder.h>

#include <Engine/Graphics/cVertexFormat.h>
#include <Engine/Graphics/VertexFormats.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Graphics/Configuration.h>
#include <Engine/Graphics/Mesh.h>
//#include <External/Lua/Includes.h>

// Class Declaration
//==================

namespace eae6320
{
	namespace Assets
	{
		class cMeshBuilder final : public iBuilder
		{
			// Inherited Implementation
			//=========================

		private:

			// Build
			//------

			cResult Build(const std::vector<std::string>& i_arguments) final;

			// Implementation
			//===============

			// Lua Mesh Data
			//--------------

			cResult LoadTableValues_vertices(lua_State& io_luaState, Graphics::VertexFormats::sVertex_mesh*& i_vertexData, unsigned int& i_vertexCount);
			cResult LoadTableValues_vertices_vertex(lua_State& io_luaState, Graphics::VertexFormats::sVertex_mesh*& i_vertexData, unsigned int& i_vertexCount);
			cResult LoadTableValues_vertices_vertex_coordinates(lua_State& io_luaState, Graphics::VertexFormats::sVertex_mesh& i_vertexData);

			cResult LoadTableValues_indices(lua_State& io_luaState, uint16_t*& i_indexData, unsigned int& i_indexCount);
			cResult LoadTableValues_indices_index(lua_State& io_luaState, uint16_t*& i_indexData, unsigned int& i_indexCount);

			cResult WriteToBinaryFile(Graphics::VertexFormats::sVertex_mesh* i_vertexData, uint16_t* i_indexData, const unsigned int i_vertexCount, const unsigned int i_indexCount, const char* const i_path);
		};
	}
}

#endif	// EAE6320_CSHADERBUILDER_H
