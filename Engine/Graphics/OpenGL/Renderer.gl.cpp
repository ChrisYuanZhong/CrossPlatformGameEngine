// Includes
//=========

#include "../Renderer.h"

#include "../sContext.h"

#include <Engine/Asserts/Asserts.h>

#include <new>

// Helper Declarations
//====================

namespace
{
	eae6320::cResult InitializeShadingData();
}

void eae6320::Graphics::Renderer::RenderFrame(sDataRequiredToRenderAFrame* s_dataBeingRenderedByRenderThread, eae6320::Graphics::cConstantBuffer& s_constantBuffer_frame)
{
	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	{
		// Black is usually used
		{
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}
		{
			constexpr GLbitfield clearColor = GL_COLOR_BUFFER_BIT;
			glClear(clearColor);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}
	}
	// In addition to the color buffer there is also a hidden image called the "depth buffer"
	// which is used to make it less important which order draw calls are made.
	// It must also be "cleared" every frame just like the visible color buffer.
	{
		{
			glDepthMask(GL_TRUE);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
			constexpr GLclampd clearToFarDepth = 1.0;
			glClearDepth(clearToFarDepth);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}
		{
			constexpr GLbitfield clearDepth = GL_DEPTH_BUFFER_BIT;
			glClear(clearDepth);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}
	}

	EAE6320_ASSERT(s_dataBeingRenderedByRenderThread);

	// Update the frame constant buffer
	{
		// Copy the data from the system memory that the application owns to GPU memory
		auto& constantData_frame = s_dataBeingRenderedByRenderThread->constantData_frame;
		s_constantBuffer_frame.Update(&constantData_frame);
	}

	s_effect.BindShadingData();

	s_mesh.DrawGeometry();

	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it the contents of the back buffer must be "presented"
	// (or "swapped" with the "front buffer", which is the image that is actually being displayed)
	{
		const auto deviceContext = sContext::g_context.deviceContext;
		EAE6320_ASSERT(deviceContext != NULL);
		const auto glResult = SwapBuffers(deviceContext);
		EAE6320_ASSERT(glResult != FALSE);
	}
}

eae6320::cResult eae6320::Graphics::Renderer::Initialize(eae6320::cResult& result, const sInitializationParameters& i_initializationParameters)
{
	// Static Data Initialization
	eae6320::Graphics::VertexFormats::sVertex_mesh vertexData[3];
	{
		// Direct3D is left-handed

		// Draw a house shape using three triangles:
		vertexData[0].x = -0.5f;
		vertexData[0].y = -0.5f;
		vertexData[0].z = 0.0f;

		vertexData[1].x = -0.5f;
		vertexData[1].y = 0.5f;
		vertexData[1].z = 0.0f;

		vertexData[2].x = 0.5f;
		vertexData[2].y = 0.5f;
		vertexData[2].z = 0.0f;

	}

	uint16_t indexData[3] = { 0, 1, 2 };
	// Initialize the shading data
	{
		if (!(result = InitializeShadingData("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/animatedcolor.shader")))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}
	}
	// Initialize the geometry
	{
		if (!(result = s_mesh.InitializeGeometry(vertexData, indexData, 3, 3)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return result;
		}
	}

	return result;
}

void eae6320::Graphics::Renderer::CleanUp(eae6320::cResult& result)
{
	s_mesh.CleanUp(result);

	s_effect.CleanUpProgram(result);
	s_effect.CleanUp();
}

// Helper Definitions
//===================

eae6320::cResult eae6320::Graphics::Renderer::InitializeShadingData(const char* const vertexShaderPath, const char* const fragmentShaderPath)
{
	auto result = s_effect.InitializeShadingData(vertexShaderPath, fragmentShaderPath);

	return s_effect.CreateProgram(result);
}