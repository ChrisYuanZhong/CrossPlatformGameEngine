#pragma once

// Includes
//=========

#include "Graphics.h"

#include "Includes.h"
#include "cVertexFormat.h"
#include "VertexFormats.h"

namespace eae6320
{
	namespace Graphics
	{
		class Mesh
		{
		public:
			void DrawGeometry(ID3D11DeviceContext* const direct3dImmediateContext);

			// Initialize / Clean Up
			//----------------------

			eae6320::cResult InitializeGeometry();
			void CleanUp();

		private:
			// Geometry Data
			//--------------

			eae6320::Graphics::cVertexFormat* s_vertexFormat = nullptr;

			// A vertex buffer holds the data for each vertex
			ID3D11Buffer* s_vertexBuffer = nullptr;
		};
	}
}