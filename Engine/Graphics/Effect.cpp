// Includes
//=========

#include "Effect.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Logging/Logging.h>

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

	if (!(result = eae6320::Graphics::cShader::Load(vertexShaderPath,
		vertexShader, eae6320::Graphics::eShaderType::Vertex)))
	{
		EAE6320_ASSERTF(false, "Can't initialize shading data without vertex shader");
		return result;
	}
	if (!(result = eae6320::Graphics::cShader::Load(fragmentShaderPath,
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

eae6320::cResult eae6320::Graphics::Effect::Load(Effect*& o_effect, const char* const vertexShaderPath, const char* const fragmentShaderPath)
{
	auto result = Results::Success;

	Effect* newEffect = nullptr;
	cScopeGuard cScopeGuard([&o_effect, &result, &newEffect]
		{
			if (result)
			{
				EAE6320_ASSERT(newEffect != nullptr);
				o_effect = newEffect;
			}
			else
			{
				if (newEffect)
				{
					newEffect->DecrementReferenceCount();
					newEffect = nullptr;
				}
				o_effect = nullptr;
			}
		});

	// Allocate a new effect
	{
		newEffect = new (std::nothrow) Effect();
		if (!newEffect)
		{
			result = Results::OutOfMemory;
			EAE6320_ASSERTF(false, "Couldn't allocate memory for the effect");
			Logging::OutputError("Failed to allocate memory for effect");
			return result;
		}
	}

	// Initialize the new effect
	{
		if (!(result = newEffect->InitializeShadingData(vertexShaderPath, fragmentShaderPath)))
		{
			EAE6320_ASSERTF(false, "Initialization of new effect failed");
			return result;
		}
	}

	return result;
}

eae6320::Graphics::Effect::~Effect()
{
	cResult result = Results::Success;
	CleanUp(result);
	EAE6320_ASSERT(result);
}