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
#elif defined( EAE6320_PLATFORM_GL )
			void CleanUpProgram(eae6320::cResult& result);
			eae6320::cResult CreateProgram(eae6320::cResult& result);
#endif
			void BindShadingData();
			void CleanUp(eae6320::cResult& result);
			eae6320::cResult InitializeShadingData(const char* const vertexShaderPath, const char* const fragmentShaderPath);

		private:
			eae6320::Graphics::cShader* vertexShader = nullptr;
			eae6320::Graphics::cShader* fragmentShader = nullptr;

			eae6320::Graphics::cRenderState renderState;
			// Shading Data
			//-------------

		#if defined( EAE6320_PLATFORM_D3D )
		#elif defined( EAE6320_PLATFORM_GL )
			GLuint programId = 0;
		#endif
		};
	}
}