#pragma once

// Includes
//=========

#if defined( EAE6320_PLATFORM_D3D )
	#include "Direct3D/Includes.h"
#elif defined( EAE6320_PLATFORM_GL )
	#include "OpenGL/Includes.h"
#endif

#include <string>
#include "cVertexFormat.h"
#include "VertexFormats.h"

#include <Engine/Assets/ReferenceCountedAssets.h>
#include <External/Lua/Includes.h>

namespace eae6320
{
	namespace Graphics
	{
		class Mesh
		{

			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(Mesh)

		public:
			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS()

				static cResult Load(Mesh*& o_mesh, VertexFormats::sVertex_mesh* i_vertexData, uint16_t* i_indexData, const unsigned int i_vertexCount, const unsigned int i_indexCount);
				static cResult LoadFromFile(Mesh*& o_mesh, const char* const meshPath);

		#if defined( EAE6320_PLATFORM_D3D )
		#elif defined( EAE6320_PLATFORM_GL )
				void ConvertLeftHandedToRight(uint16_t* const& i_indexData);
		#endif

			void DrawGeometry();

		private:
			Mesh() = default;
			~Mesh();
			eae6320::cResult InitializeGeometry(VertexFormats::sVertex_mesh* i_vertexData, uint16_t* i_indexData, const unsigned int i_vertexCount, const unsigned int i_indexCount);
			void CleanUp(eae6320::cResult& result);

			EAE6320_ASSETS_DECLAREREFERENCECOUNT()

			unsigned int indexCount = 0;

			// Lua Mesh Data
			//--------------

			static eae6320::cResult LoadTableValues_vertices(lua_State& io_luaState, VertexFormats::sVertex_mesh*& i_vertexData, unsigned int& i_vertexCount);
			static eae6320::cResult LoadTableValues_vertices_vertex(lua_State& io_luaState, VertexFormats::sVertex_mesh*& i_vertexData, unsigned int& i_vertexCount);
			static eae6320::cResult LoadTableValues_vertices_vertex_coordinates(lua_State& io_luaState, VertexFormats::sVertex_mesh& i_vertexData);

			static eae6320::cResult LoadTableValues_indices(lua_State& io_luaState, uint16_t*& i_indexData, unsigned int& i_indexCount);
			static eae6320::cResult LoadTableValues_indices_index(lua_State& io_luaState, uint16_t*& i_indexData, unsigned int& i_indexCount);

			// Geometry Data
			//--------------

		#if defined( EAE6320_PLATFORM_D3D )
			eae6320::Graphics::cVertexFormat* s_vertexFormat = nullptr;

			// A vertex buffer holds the data for each vertex
			ID3D11Buffer* vertexBuffer = nullptr;
			// An index buffer holds the indices for each triangle that needs to be drawn
			ID3D11Buffer* indexBuffer = nullptr;
		#elif defined( EAE6320_PLATFORM_GL )
			// A vertex buffer holds the data for each vertex
			GLuint vertexBufferId = 0;
			// A vertex array encapsulates the vertex data as well as the vertex input layout
			GLuint vertexArrayId = 0;
			// An index buffer holds the indices for each triangle that needs to be drawn
			GLuint indexBufferId = 0;
		#endif
		};
	}
}