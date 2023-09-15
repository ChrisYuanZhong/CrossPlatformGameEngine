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
				EAE6320_ASSERT((vertexShader != nullptr) && (vertexShader->m_shaderObject.vertex != nullptr));
				direct3dImmediateContext->VSSetShader(vertexShader->m_shaderObject.vertex, noInterfaces, interfaceCount);
			}
			// Fragment shader
			{
				EAE6320_ASSERT((fragmentShader != nullptr) && (fragmentShader->m_shaderObject.vertex != nullptr));
				direct3dImmediateContext->PSSetShader(fragmentShader->m_shaderObject.fragment, noInterfaces, interfaceCount);
			}
		}
		// Render state
		{
			renderState.Bind();
		}
	}
}