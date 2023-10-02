// Includes
//=========

#include "cMyGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>

#include <Engine/Logging/Logging.h>

// Inherited Implementation
//=========================

// Run
//----

void eae6320::cMyGame::UpdateBasedOnInput()
{
	isLeftDown = false;
	isRightDown = false;
	isUpDown = false;
	isDownDown = false;

	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		eae6320::Logging::OutputMessage("Escape Pressed: Exiting the application");
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left))
	{
		eae6320::Logging::OutputMessage("Left Key Pressed");
		isLeftDown = true;
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right))
	{
		eae6320::Logging::OutputMessage("Right Key Pressed");
		isRightDown = true;
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Up))
	{
		eae6320::Logging::OutputMessage("Up Key Pressed");
		isUpDown = true;
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Down))
	{
		eae6320::Logging::OutputMessage("Down Key Pressed");
		isDownDown = true;
	}
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::cMyGame::Initialize()
{
	eae6320::Logging::OutputMessage("Initializing Chris Yuan Zhong's EAE6320 Example Game");

	auto result = Results::Success;

	// Initialize the shading data
	{
		if (!(result = eae6320::Graphics::Effect::Load(originalMeshEffectPairs[0].effect, "data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/animatedcolor1.shader")))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}
		if (!(result = eae6320::Graphics::Effect::Load(originalMeshEffectPairs[1].effect, "data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/animatedcolor2.shader")))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}
	}
	// Initialize the geometry
	{
		// Static Data Initialization
		eae6320::Graphics::VertexFormats::sVertex_mesh vertexData1[3];
		{
			// Direct3D is left-handed

			// Draw a house shape using three triangles:
			vertexData1[0].x = -0.5f;
			vertexData1[0].y = -0.5f;
			vertexData1[0].z = 0.0f;

			vertexData1[1].x = -0.5f;
			vertexData1[1].y = 0.5f;
			vertexData1[1].z = 0.0f;

			vertexData1[2].x = 0.5f;
			vertexData1[2].y = 0.5f;
			vertexData1[2].z = 0.0f;

		}

		uint16_t indexData1[3] = { 0, 1, 2 };

		if (!(result = eae6320::Graphics::Mesh::Load(originalMeshEffectPairs[0].mesh, vertexData1, indexData1, 3, 3)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return result;
		}

		// Static Data Initialization
		eae6320::Graphics::VertexFormats::sVertex_mesh vertexData2[3];
		{
			// Direct3D is left-handed

			// Draw a house shape using three triangles:
			vertexData2[0].x = -0.5f;
			vertexData2[0].y = -0.5f;
			vertexData2[0].z = 0.0f;

			vertexData2[1].x = 0.5f;
			vertexData2[1].y = 0.5f;
			vertexData2[1].z = 0.0f;

			vertexData2[2].x = 0.5f;
			vertexData2[2].y = -0.5f;
			vertexData2[2].z = 0.0f;

		}

		uint16_t indexData2[3] = { 0, 1, 2 };

		if (!(result = eae6320::Graphics::Mesh::Load(originalMeshEffectPairs[1].mesh, vertexData2, indexData2, 3, 3)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return result;
		}
	}

	return Results::Success;
}

eae6320::cResult eae6320::cMyGame::CleanUp()
{
	eae6320::Logging::OutputMessage("Cleaning up Chris Yuan Zhong's EAE6320 Example Game");

	auto result = Results::Success;

	for (unsigned int i = 0; i < numPairs; i++)
	{
		meshEffectPairs[i].mesh->DecrementReferenceCount();
		meshEffectPairs[i].effect->DecrementReferenceCount();
	}

	return result;
}

void eae6320::cMyGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	Graphics::SetClearColor(0x3f82f7ff);

	unsigned int numToDraw = numPairs;
	
	if (!(isLeftDown || isRightDown || isUpDown || isDownDown))
	{
		numToDraw = 2;
		for (unsigned int i = 0; i < numPairs; i++)
		{
			meshEffectPairs[i].mesh = originalMeshEffectPairs[i].mesh;
			meshEffectPairs[i].effect = originalMeshEffectPairs[i].effect;
		}
	}
	// While Left is down, only draw the first mesh
	else if (isLeftDown)
	{
		numToDraw = 1;
		meshEffectPairs[0].mesh = originalMeshEffectPairs[0].mesh;
		meshEffectPairs[0].effect = originalMeshEffectPairs[0].effect;
	}
	// While Right is down, only draw the second mesh
	else if (isRightDown)
	{
		numToDraw = 1;
		meshEffectPairs[0].mesh = originalMeshEffectPairs[1].mesh;
		meshEffectPairs[0].effect = originalMeshEffectPairs[1].effect;
	}
	// While Up is down, change the effect of the first mesh to be the second effect
	else if (isUpDown)
	{
		numToDraw = 2;
		meshEffectPairs[0].mesh = originalMeshEffectPairs[0].mesh;
		meshEffectPairs[0].effect = originalMeshEffectPairs[1].effect;
		meshEffectPairs[1].mesh = originalMeshEffectPairs[1].mesh;
		meshEffectPairs[1].effect = originalMeshEffectPairs[1].effect;
	}
	// While Down is down, change the effect of the second mesh to be the first effect
	else if (isDownDown)
	{
		numToDraw = 2;
		meshEffectPairs[0].mesh = originalMeshEffectPairs[0].mesh;
		meshEffectPairs[0].effect = originalMeshEffectPairs[0].effect;
		meshEffectPairs[1].mesh = originalMeshEffectPairs[1].mesh;
		meshEffectPairs[1].effect = originalMeshEffectPairs[0].effect;
	}

	Graphics::SubmitMeshEffectLocationTrios(meshEffectPairs, numToDraw);
}
