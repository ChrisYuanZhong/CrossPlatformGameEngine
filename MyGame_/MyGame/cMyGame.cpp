// Includes
//=========

#include "cMyGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Math/cQuaternion.h>

#include <Engine/Logging/Logging.h>

// Inherited Implementation
//=========================

// Run
//----

void eae6320::cMyGame::UpdateBasedOnInput()
{
	gameInputs.Reset();

	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		eae6320::Logging::OutputMessage("Escape Pressed: Exiting the application");
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}
	if (UserInput::IsKeyPressed('W'))
	{
		gameInputs.isWDown = true;
	}
	if (UserInput::IsKeyPressed('A'))
	{
		gameInputs.isADown = true;
	}
	if (UserInput::IsKeyPressed('S'))
	{
		gameInputs.isSDown = true;
	}
	if (UserInput::IsKeyPressed('D'))
	{
		gameInputs.isDDown = true;
	}
	if (UserInput::IsKeyPressed('1'))
	{
		gameInputs.is1Down = true;
	}
	if (UserInput::IsKeyPressed('2'))
	{
		gameInputs.is2Down = true;
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Up))
	{
		gameInputs.isUpArrowDown = true;
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Down))
	{
		gameInputs.isDownArrowDown = true;
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left))
	{
		gameInputs.isLeftArrowDown = true;
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right))
	{
		gameInputs.isRightArrowDown = true;
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
		if (!(result = eae6320::Graphics::Effect::Load(effects[0], "data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/animatedcolor1.shader")))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}
		if (!(result = eae6320::Graphics::Effect::Load(effects[1], "data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/standard.shader")))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}
	}
	// Initialize the geometry
	{
		//// Static Data Initialization
		//eae6320::Graphics::VertexFormats::sVertex_mesh vertexData1[4];
		//{
		//	// Direct3D is left-handed

		//	// Draw a house shape using three triangles:
		//	vertexData1[0].x = -0.5f;
		//	vertexData1[0].y = -0.5f;
		//	vertexData1[0].z = 0.0f;

		//	vertexData1[1].x = -0.5f;
		//	vertexData1[1].y = 0.5f;
		//	vertexData1[1].z = 0.0f;

		//	vertexData1[2].x = 0.5f;
		//	vertexData1[2].y = 0.5f;
		//	vertexData1[2].z = 0.0f;

		//	vertexData1[3].x = 0.5f;
		//	vertexData1[3].y = -0.5f;
		//	vertexData1[3].z = 0.0f;

		//}

		//uint16_t indexData1[6] = { 0, 1, 2, 0, 2, 3 };

		//if (!(result = eae6320::Graphics::Mesh::Load(meshes[0], vertexData1, indexData1, 4, 6)))
		//{
		//	EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
		//	return result;
		//}


		//// Static Data Initialization
		//eae6320::Graphics::VertexFormats::sVertex_mesh vertexData2[3];
		//{
		//	// Direct3D is left-handed

		//	// Draw a house shape using three triangles:
		//	vertexData2[0].x = -0.5f;
		//	vertexData2[0].y = -0.5f;
		//	vertexData2[0].z = 0.0f;

		//	vertexData2[1].x = 0.0f;
		//	vertexData2[1].y = 0.5f;
		//	vertexData2[1].z = 0.0f;

		//	vertexData2[2].x = 0.5f;
		//	vertexData2[2].y = -0.5f;
		//	vertexData2[2].z = 0.0f;

		//}

		//uint16_t indexData2[3] = { 0, 1, 2 };

		//if (!(result = eae6320::Graphics::Mesh::Load(meshes[1], vertexData2, indexData2, 3, 3)))
		//{
		//	EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
		//	return result;
		//}

		if (!(result = eae6320::Graphics::Mesh::LoadFromFile(meshes[0], "data/Meshes/Plane.mesh")))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return result;
		}

		if (!(result = eae6320::Graphics::Mesh::LoadFromFile(meshes[1], "data/Meshes/Snowman.mesh")))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return result;
		}

		if (!(result = eae6320::Graphics::Mesh::LoadFromFile(meshes[2], "data/Meshes/Helix.mesh")))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return result;
		}

		gameObjectsToBeRendered[0].SetMesh(meshes[1]);
		gameObjectsToBeRendered[0].SetEffect(effects[0]);
		gameObjectsToBeRendered[0].SetPosition(Math::sVector(0.0f, -1.0f, 0.0f));
		gameObjectsToBeRendered[1].SetMesh(meshes[0]);
		gameObjectsToBeRendered[1].SetEffect(effects[1]);
		gameObjectsToBeRendered[1].SetPosition(Math::sVector(0.0f, -1.0f, 0.0f));
	}

	// Set the main camera here
	mainCamera = &camera;

	return Results::Success;
}

eae6320::cResult eae6320::cMyGame::CleanUp()
{
	eae6320::Logging::OutputMessage("Cleaning up Chris Yuan Zhong's EAE6320 Example Game");

	auto result = Results::Success;

	if (isMeshModified)
	{
		meshes[0]->DecrementReferenceCount();
		effects[0]->DecrementReferenceCount();
	}
	else
	{
		meshes[1]->DecrementReferenceCount();
		effects[1]->DecrementReferenceCount();
	}

	return result;
}

void eae6320::cMyGame::UpdateSimulationBasedOnInput()
{
	if (gameInputs.is1Down)
	{
		isMeshModified = false;
		gameObjectsToBeRendered[0].SetMesh(meshes[1]);
		gameObjectsToBeRendered[0].SetEffect(effects[0]);
	}
	if (gameInputs.is2Down)
	{
		isMeshModified = true;
		gameObjectsToBeRendered[0].SetMesh(meshes[2]);
		gameObjectsToBeRendered[0].SetEffect(effects[1]);
	}

	//gameObjects[0].SetPosition(Math::sVector(0.0f, 0.0f, 0.0f));
	//gameObjects[0].SetOrientation(Math::cQuaternion(1.0f, Math::sVector(0.0f, 0.0f, 1.0f)));

	if (gameInputs.isWDown)
	{
		gameObjectsToBeRendered[0].SetVelocity(gameObjectsToBeRendered[0].GetVelocity() + Math::sVector(0.0f, velocity, 0.0f));
	}
	if (gameInputs.isADown)
	{
		gameObjectsToBeRendered[0].SetVelocity(gameObjectsToBeRendered[0].GetVelocity() + Math::sVector(-velocity, 0.0f, 0.0f));
	}
	if (gameInputs.isSDown)
	{
		gameObjectsToBeRendered[0].SetVelocity(gameObjectsToBeRendered[0].GetVelocity() + Math::sVector(0.0f, -velocity, 0.0f));
	}
	if (gameInputs.isDDown)
	{
		gameObjectsToBeRendered[0].SetVelocity(gameObjectsToBeRendered[0].GetVelocity() + Math::sVector(velocity, 0.0f, 0.0f));
	}

	if (gameInputs.isUpArrowDown)
	{
		mainCamera->SetVelocity(mainCamera->GetVelocity() + Math::sVector(0.0f, 0.0f, -velocity));
	}
	if (gameInputs.isDownArrowDown)
	{
		mainCamera->SetVelocity(mainCamera->GetVelocity() + Math::sVector(0.0f, 0.0f, velocity));
	}
	if (gameInputs.isLeftArrowDown)
	{
		mainCamera->SetVelocity(mainCamera->GetVelocity() + Math::sVector(-velocity, 0.0f, 0.0f));
	}
	if (gameInputs.isRightArrowDown)
	{
		mainCamera->SetVelocity(mainCamera->GetVelocity() + Math::sVector(velocity, 0.0f, 0.0f));
	}
}

void eae6320::cMyGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	// Update the game objects
	for (unsigned int i = 0; i < numGameObjectsToBeRendered; i++)
	{
		gameObjectsToBeRendered[i].Update(i_elapsedSecondCount_sinceLastUpdate);
	}

	mainCamera->Update(i_elapsedSecondCount_sinceLastUpdate);
}

void eae6320::cMyGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	Graphics::SetClearColor(0x3f82f7ff);

	Graphics::MeshEffectLocationTrio meshEffectLocationTrios[numGameObjectsToBeRendered]{};

	for (unsigned int i = 0; i < numGameObjectsToBeRendered; i++)
	{
		meshEffectLocationTrios[i].mesh = gameObjectsToBeRendered[i].GetMesh();
		meshEffectLocationTrios[i].effect = gameObjectsToBeRendered[i].GetEffect();
		meshEffectLocationTrios[i].constantData_drawCall.g_transform_localToWorld =
			gameObjectsToBeRendered[i].GetLocalToWorldTransformPrediction(i_elapsedSecondCount_sinceLastSimulationUpdate);
	}

	Graphics::SubmitMeshEffectLocationTrios(meshEffectLocationTrios, numGameObjectsToBeRendered);

	Graphics::SubmitCameraData(mainCamera->GetWorldToCameraTransformPrediction(i_elapsedSecondCount_sinceLastSimulationUpdate), mainCamera->GetCameraToProjectedTransform());
}
