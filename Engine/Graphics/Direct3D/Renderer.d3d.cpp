// Includes
//=========

#include "../Renderer.h"

#include "../cVertexFormat.h"
#include "../sContext.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/ScopeGuard/cScopeGuard.h>

// Helper Declarations
//====================

namespace
{
	eae6320::cResult InitializeViews(const unsigned int i_resolutionWidth, const unsigned int i_resolutionHeight);
}

void eae6320::Graphics::Renderer::RenderFrame(sDataRequiredToRenderAFrame* s_dataBeingRenderedByRenderThread, eae6320::Graphics::cConstantBuffer& s_constantBuffer_frame)
{
	auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;
	EAE6320_ASSERT(direct3dImmediateContext);

	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	{
		EAE6320_ASSERT(s_renderTargetView);

		// Black is usually used
		constexpr float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		direct3dImmediateContext->ClearRenderTargetView(s_renderTargetView, clearColor);
	}
	// In addition to the color buffer there is also a hidden image called the "depth buffer"
	// which is used to make it less important which order draw calls are made.
	// It must also be "cleared" every frame just like the visible color buffer.
	{
		EAE6320_ASSERT(s_depthStencilView);

		constexpr float clearToFarDepth = 1.0f;
		constexpr uint8_t stencilValue = 0;	// Arbitrary if stencil isn't used
		direct3dImmediateContext->ClearDepthStencilView(s_depthStencilView, D3D11_CLEAR_DEPTH, clearToFarDepth, stencilValue);
	}

	EAE6320_ASSERT(s_dataBeingRenderedByRenderThread);
	auto* const dataRequiredToRenderFrame = s_dataBeingRenderedByRenderThread;

	// Update the frame constant buffer
	{
		// Copy the data from the system memory that the application owns to GPU memory
		auto& constantData_frame = dataRequiredToRenderFrame->constantData_frame;
		s_constantBuffer_frame.Update(&constantData_frame);
	}

	s_effect.BindShadingData(direct3dImmediateContext);
	s_mesh.DrawGeometry(direct3dImmediateContext);

	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it the contents of the back buffer must be "presented"
	// (or "swapped" with the "front buffer", which is the image that is actually being displayed)
	{
		auto* const swapChain = sContext::g_context.swapChain;
		EAE6320_ASSERT(swapChain);
		constexpr unsigned int swapImmediately = 0;
		constexpr unsigned int presentNextFrame = 0;
		const auto result = swapChain->Present(swapImmediately, presentNextFrame);
		EAE6320_ASSERT(SUCCEEDED(result));
	}

	// After all of the data that was submitted for this frame has been used
	// you must make sure that it is all cleaned up and cleared out
	// so that the struct can be re-used (i.e. so that data for a new frame can be submitted to it)
	{
		// (At this point in the class there isn't anything that needs to be cleaned up)
		//dataRequiredToRenderFrame	// TODO
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
	// Initialize the views
	{
		if (!(result = InitializeViews(i_initializationParameters.resolutionWidth, i_initializationParameters.resolutionHeight)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the views");
			return result;
		}
	}
	// Initialize the shading data
	{
		if (!(result = s_effect.InitializeShadingData("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/animatedcolor.shader")))
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
	if (s_renderTargetView)
	{
		s_renderTargetView->Release();
		s_renderTargetView = nullptr;
	}
	if (s_depthStencilView)
	{
		s_depthStencilView->Release();
		s_depthStencilView = nullptr;
	}

	s_mesh.CleanUp();

	s_effect.CleanUp();
}

// Helper Definitions
//===================

eae6320::cResult eae6320::Graphics::Renderer::InitializeViews(const unsigned int i_resolutionWidth, const unsigned int i_resolutionHeight)
{
	auto result = eae6320::Results::Success;

	ID3D11Texture2D* backBuffer = nullptr;
	ID3D11Texture2D* depthBuffer = nullptr;
	eae6320::cScopeGuard scopeGuard([&backBuffer, &depthBuffer]
		{
			// Regardless of success or failure the two texture resources should be released
			// (if the function is successful the views will hold internal references to the resources)
			if (backBuffer)
			{
				backBuffer->Release();
				backBuffer = nullptr;
			}
			if (depthBuffer)
			{
				depthBuffer->Release();
				depthBuffer = nullptr;
			}
		});

	auto& g_context = eae6320::Graphics::sContext::g_context;
	auto* const direct3dDevice = g_context.direct3dDevice;
	EAE6320_ASSERT(direct3dDevice);
	auto* const direct3dImmediateContext = g_context.direct3dImmediateContext;
	EAE6320_ASSERT(direct3dImmediateContext);

	// Create a "render target view" of the back buffer
	// (the back buffer was already created by the call to D3D11CreateDeviceAndSwapChain(),
	// but a "view" of it is required to use as a "render target",
	// meaning a texture that the GPU can render to)
	{
		// Get the back buffer from the swap chain
		{
			constexpr unsigned int bufferIndex = 0;	// This must be 0 since the swap chain is discarded
			const auto d3dResult = g_context.swapChain->GetBuffer(bufferIndex, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
			if (FAILED(d3dResult))
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, "Couldn't get the back buffer from the swap chain (HRESULT %#010x)", d3dResult);
				eae6320::Logging::OutputError("Direct3D failed to get the back buffer from the swap chain (HRESULT %#010x)", d3dResult);
				return result;
			}
		}
		// Create the view
		{
			constexpr D3D11_RENDER_TARGET_VIEW_DESC* const accessAllSubResources = nullptr;
			const auto d3dResult = direct3dDevice->CreateRenderTargetView(backBuffer, accessAllSubResources, &s_renderTargetView);
			if (FAILED(d3dResult))
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, "Couldn't create render target view (HRESULT %#010x)", d3dResult);
				eae6320::Logging::OutputError("Direct3D failed to create the render target view (HRESULT %#010x)", d3dResult);
				return result;
			}
		}
	}
	// Create a depth/stencil buffer and a view of it
	{
		// Unlike the back buffer no depth/stencil buffer exists until and unless it is explicitly created
		{
			const auto textureDescription = [i_resolutionWidth, i_resolutionHeight]
			{
				auto textureDescription = []() constexpr
				{
					D3D11_TEXTURE2D_DESC textureDescription{};

					textureDescription.MipLevels = 1;	// A depth buffer has no MIP maps
					textureDescription.ArraySize = 1;
					textureDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// 24 bits for depth and 8 bits for stencil
					{
						DXGI_SAMPLE_DESC& sampleDescription = textureDescription.SampleDesc;

						sampleDescription.Count = 1;	// No multisampling
						sampleDescription.Quality = 0;	// Doesn't matter when Count is 1
					}
					textureDescription.Usage = D3D11_USAGE_DEFAULT;	// Allows the GPU to write to it
					textureDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
					textureDescription.CPUAccessFlags = 0;	// The CPU doesn't need access
					textureDescription.MiscFlags = 0;

					return textureDescription;
				}();

				textureDescription.Width = i_resolutionWidth;
				textureDescription.Height = i_resolutionHeight;

				return textureDescription;
			}();
			// The GPU renders to the depth/stencil buffer and so there is no initial data
			// (like there would be with a traditional texture loaded from disk)
			constexpr D3D11_SUBRESOURCE_DATA* const noInitialData = nullptr;
			const auto d3dResult = direct3dDevice->CreateTexture2D(&textureDescription, noInitialData, &depthBuffer);
			if (FAILED(d3dResult))
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, "Couldn't create depth buffer (HRESULT %#010x)", d3dResult);
				eae6320::Logging::OutputError("Direct3D failed to create the depth buffer resource (HRESULT %#010x)", d3dResult);
				return result;
			}
		}
		// Create the view
		{
			constexpr D3D11_DEPTH_STENCIL_VIEW_DESC* const noSubResources = nullptr;
			const auto d3dResult = direct3dDevice->CreateDepthStencilView(depthBuffer, noSubResources, &s_depthStencilView);
			if (FAILED(d3dResult))
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, "Couldn't create depth stencil view (HRESULT %#010x)", d3dResult);
				eae6320::Logging::OutputError("Direct3D failed to create the depth stencil view (HRESULT %#010x)", d3dResult);
				return result;
			}
		}
	}

	// Bind the views
	{
		constexpr unsigned int renderTargetCount = 1;
		direct3dImmediateContext->OMSetRenderTargets(renderTargetCount, &s_renderTargetView, s_depthStencilView);
	}
	// Specify that the entire render target should be visible
	{
		const auto viewPort = [i_resolutionWidth, i_resolutionHeight]
		{
			auto viewPort = []() constexpr
			{
				D3D11_VIEWPORT viewPort{};

				viewPort.TopLeftX = viewPort.TopLeftY = 0.0f;
				viewPort.MinDepth = 0.0f;
				viewPort.MaxDepth = 1.0f;

				return viewPort;
			}();
			viewPort.Width = static_cast<float>(i_resolutionWidth);
			viewPort.Height = static_cast<float>(i_resolutionHeight);

			return viewPort;
		}();
		constexpr unsigned int viewPortCount = 1;
		direct3dImmediateContext->RSSetViewports(viewPortCount, &viewPort);
	}

	return result;
}