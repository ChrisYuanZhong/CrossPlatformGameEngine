#pragma once

// Includes
//=========

#include "Graphics.h"

#if defined( EAE6320_PLATFORM_D3D )
#include "Direct3D/Includes.h"
#elif defined( EAE6320_PLATFORM_GL )
#include "OpenGL/Includes.h"
#endif

#include "cRenderState.h"
#include "cShader.h"

namespace eae6320
{
	namespace Graphics
	{
		class Effect
		{
		public:
#if defined( EAE6320_PLATFORM_D3D )
			void BindShadingData(ID3D11DeviceContext* const direct3dImmediateContext);
#elif defined( EAE6320_PLATFORM_GL )
			void BindShadingData();
			void CleanUpProgram(eae6320::cResult& result);
			eae6320::cResult CreateProgram(eae6320::cResult& result);
#endif

			void CleanUp();
			eae6320::cResult InitializeShadingData();

		private:
			// Shading Data
			//-------------

#if defined( EAE6320_PLATFORM_D3D )
			eae6320::Graphics::cShader* s_vertexShader = nullptr;
			eae6320::Graphics::cShader* s_fragmentShader = nullptr;

			eae6320::Graphics::cRenderState s_renderState;
#elif defined( EAE6320_PLATFORM_GL )
			eae6320::Graphics::cShader* s_vertexShader = nullptr;
			eae6320::Graphics::cShader* s_fragmentShader = nullptr;
			static GLuint s_programId;

			eae6320::Graphics::cRenderState s_renderState;
#endif
		};
	}
}