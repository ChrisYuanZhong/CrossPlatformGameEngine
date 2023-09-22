#pragma once

// Includes
//=========

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

			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(Effect)

		public:
			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS()

#if defined( EAE6320_PLATFORM_D3D )
#elif defined( EAE6320_PLATFORM_GL )
			void CleanUpProgram(eae6320::cResult& result);
			eae6320::cResult CreateProgram(eae6320::cResult& result);
#endif
			static eae6320::cResult Load(Effect*& o_effect, const char* const vertexShaderPath, const char* const fragmentShaderPath);
			void BindShadingData();

		private:
			Effect() = default;
			~Effect();
			eae6320::cResult InitializeShadingData(const char* const vertexShaderPath, const char* const fragmentShaderPath);
			void CleanUp(eae6320::cResult& result);
			
			EAE6320_ASSETS_DECLAREREFERENCECOUNT()

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