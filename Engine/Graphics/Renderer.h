#pragma once

// Includes
//=========

#include "Graphics.h"
#include "Mesh.h"
#include "Effect.h"

#include "cConstantBuffer.h"

#include <Engine/Graphics/Color.h>

#if defined( EAE6320_PLATFORM_D3D )
#include "Direct3D/Includes.h"
#elif defined( EAE6320_PLATFORM_GL )
#include "OpenGL/Includes.h"
#endif

namespace eae6320
{
	namespace Graphics
	{
		class Renderer
		{
		public:
#if defined( EAE6320_PLATFORM_D3D )
#elif defined( EAE6320_PLATFORM_GL )
#endif

			void RenderFrame(unsigned int i_clearColor);
			void SwapBuffer();
			eae6320::cResult InitializeViews(const sInitializationParameters& i_initializationParameters);
			void CleanUp(eae6320::cResult& result);

		private:
			// Static Data
			//============

#if defined( EAE6320_PLATFORM_D3D )
			// In Direct3D "views" are objects that allow a texture to be used a particular way:
			// A render target view allows a texture to have color rendered to it
			ID3D11RenderTargetView* s_renderTargetView = nullptr;
			// A depth/stencil view allows a texture to have depth rendered to it
			ID3D11DepthStencilView* s_depthStencilView = nullptr;
#elif defined( EAE6320_PLATFORM_GL )
			
#endif
		};
	}
}