// Includes
//=========

#include "Effect.h"

void eae6320::Graphics::Effect::CleanUp(eae6320::cResult& result)
{
#if defined( EAE6320_PLATFORM_GL )
	CleanUpProgram(result);
#endif

	if (vertexShader)
	{
		vertexShader->DecrementReferenceCount();
		vertexShader = nullptr;
	}
	if (fragmentShader)
	{
		fragmentShader->DecrementReferenceCount();
		fragmentShader = nullptr;
	}
}

eae6320::cResult eae6320::Graphics::Effect::InitializeShadingData(const char* const vertexShaderPath, const char* const fragmentShaderPath)
{
	auto result = eae6320::Results::Success;

	if (!(result = eae6320::Graphics::cShader::Load("data/Shaders/Vertex/standard.shader",
		vertexShader, eae6320::Graphics::eShaderType::Vertex)))
	{
		EAE6320_ASSERTF(false, "Can't initialize shading data without vertex shader");
		return result;
	}
	if (!(result = eae6320::Graphics::cShader::Load("data/Shaders/Fragment/animatedcolor.shader",
		fragmentShader, eae6320::Graphics::eShaderType::Fragment)))
	{
		EAE6320_ASSERTF(false, "Can't initialize shading data without fragment shader");
		return result;
	}
	{
		constexpr auto renderStateBits = []
		{
			uint8_t renderStateBits = 0;

			eae6320::Graphics::RenderStates::DisableAlphaTransparency(renderStateBits);
			eae6320::Graphics::RenderStates::DisableDepthTesting(renderStateBits);
			eae6320::Graphics::RenderStates::DisableDepthWriting(renderStateBits);
			eae6320::Graphics::RenderStates::DisableDrawingBothTriangleSides(renderStateBits);

			return renderStateBits;
		}();
		if (!(result = renderState.Initialize(renderStateBits)))
		{
			EAE6320_ASSERTF(false, "Can't initialize shading data without render state");
			return result;
		}
	}

#if defined( EAE6320_PLATFORM_GL )
	return CreateProgram(result);
#endif

	return result;
}