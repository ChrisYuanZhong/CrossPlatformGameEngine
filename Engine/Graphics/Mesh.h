#pragma once

// Includes
//=========

#include "Graphics.h"

#if defined( EAE6320_PLATFORM_D3D )
	#include "Direct3D/Includes.h"
#elif defined( EAE6320_PLATFORM_GL )
	#include "OpenGL/Includes.h"
#endif

#include "cVertexFormat.h"
#include "VertexFormats.h"

namespace eae6320
{
	namespace Graphics
	{
		class Mesh
		{
		public:
		#if defined( EAE6320_PLATFORM_D3D )
			void DrawGeometry(ID3D11DeviceContext* const direct3dImmediateContext);
			void CleanUp();
		#elif defined( EAE6320_PLATFORM_GL )
			void DrawGeometry();
			void CleanUp(eae6320::cResult& result);
		#endif

			eae6320::cResult InitializeGeometry(VertexFormats::sVertex_mesh* const i_vertexData, uint16_t* const i_indexData, const unsigned int vertexCount, const unsigned int indexCount);

		private:

			// Geometry Data
			//--------------

		#if defined( EAE6320_PLATFORM_D3D )
			eae6320::Graphics::cVertexFormat* s_vertexFormat = nullptr;

			// A vertex buffer holds the data for each vertex
			ID3D11Buffer* vertexBuffer = nullptr;
			// An index buffer holds the indices for each triangle that needs to be drawn
			ID3D11Buffer* indexBuffer = nullptr;
		#elif defined( EAE6320_PLATFORM_GL )
			unsigned int indexCount;
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