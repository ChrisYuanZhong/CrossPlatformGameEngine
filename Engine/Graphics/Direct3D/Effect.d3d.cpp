// Includes
//=========

#include "../Effect.h"

void eae6320::Graphics::Effect::BindShadingData(ID3D11DeviceContext* const direct3dImmediateContext)
{
	// Bind the shading data
	{
		{
			constexpr ID3D11ClassInstance* const* noInterfaces = nullptr;
			constexpr unsigned int interfaceCount = 0;
			// Vertex shader
			{
				EAE6320_ASSERT((s_vertexShader != nullptr) && (s_vertexShader->m_shaderObject.vertex != nullptr));
				direct3dImmediateContext->VSSetShader(s_vertexShader->m_shaderObject.vertex, noInterfaces, interfaceCount);
			}
			// Fragment shader
			{
				EAE6320_ASSERT((s_fragmentShader != nullptr) && (s_fragmentShader->m_shaderObject.vertex != nullptr));
				direct3dImmediateContext->PSSetShader(s_fragmentShader->m_shaderObject.fragment, noInterfaces, interfaceCount);
			}
		}
		// Render state
		{
			s_renderState.Bind();
		}
	}
}

void eae6320::Graphics::Effect::CleanUp()
{
	if (s_vertexShader)
	{
		s_vertexShader->DecrementReferenceCount();
		s_vertexShader = nullptr;
	}
	if (s_fragmentShader)
	{
		s_fragmentShader->DecrementReferenceCount();
		s_fragmentShader = nullptr;
	}
}