// Includes
//=========

#include "SideScrollingPlatformer.h"

#include "MainCamera.h"

#include "Player.h"
#include "Plane.h"
#include "Cube.h"
#include "RotatingCube.h"
#include "Spike.h"
#include "Floor.h"
#include "Checkpoint.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Math/cQuaternion.h>

#include <Engine/Logging/Logging.h>

#include <Engine/CZPhysics/CZPhysics.h>

// Inherited Implementation
//=========================

// Run
//----

void eae6320::SideScrollingPlatformer::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		eae6320::Logging::OutputMessage("Escape Pressed: Exiting the application");
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}

	//if (UserInput::IsKeyPressed('R'))
	//{
	//	if (player)
	//	{
	//		//// remove the game object from the vector
	//		//std::vector<eae6320::Assets::GameObject*>::iterator it = gameObjectsToBeRendered.begin();
	//		//while (it != gameObjectsToBeRendered.end())
	//		//{
	//		//	eae6320::Assets::GameObject* gameObject = *it;
	//		//	if (gameObject == player)
	//		//	{
	//		//		it = gameObjectsToBeRendered.erase(it); // erase the current element and update the iterator
	//		//		break;
	//		//	}

	//		//	it++; // increment the iterator
	//		//}

	//		//delete player;
	//	}

	//	//player = new Player();
	//	//gameObjectsToBeRendered.push_back(player);

	//	//// Set the follow object of the main camera
	//	//dynamic_cast<MainCamera*>(mainCamera)->followObject = player;
	//}
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::SideScrollingPlatformer::Initialize()
{
	eae6320::Logging::OutputMessage("Initializing Chris Yuan Zhong's EAE6320 Example Game");

	auto result = Results::Success;

	Floor* floor1 = new Floor();
	gameObjectsToBeRendered.push_back(floor1);
	Floor* floor2 = new Floor();
	gameObjectsToBeRendered.push_back(floor2);
	Floor* floor3 = new Floor();
	gameObjectsToBeRendered.push_back(floor3);
	Floor* floor4 = new Floor();
	gameObjectsToBeRendered.push_back(floor4);

	player = new Player();
	gameObjectsToBeRendered.push_back(player);

	Spike* spike1 = new Spike();
	gameObjectsToBeRendered.push_back(spike1);
	Spike* spike2 = new Spike();
	gameObjectsToBeRendered.push_back(spike2);
	Spike* spike3 = new Spike();
	gameObjectsToBeRendered.push_back(spike3);
	Spike* spike4 = new Spike();
	gameObjectsToBeRendered.push_back(spike4);
	Spike* spike5 = new Spike();
	gameObjectsToBeRendered.push_back(spike5);
	Spike* spike6 = new Spike();
	gameObjectsToBeRendered.push_back(spike6);
	Spike* spike7 = new Spike();
	gameObjectsToBeRendered.push_back(spike7);
	Spike* spike8 = new Spike();
	gameObjectsToBeRendered.push_back(spike8);
	Spike* spike9 = new Spike();
	gameObjectsToBeRendered.push_back(spike9);
	Spike* spike10 = new Spike();
	gameObjectsToBeRendered.push_back(spike10);
	Spike* spike11 = new Spike();
	gameObjectsToBeRendered.push_back(spike11);
	Spike* spike12 = new Spike();
	gameObjectsToBeRendered.push_back(spike12);
	Spike* spike13 = new Spike();
	gameObjectsToBeRendered.push_back(spike13);
	Spike* spike14 = new Spike();
	gameObjectsToBeRendered.push_back(spike14);
	Spike* spike15 = new Spike();
	gameObjectsToBeRendered.push_back(spike15);
	Spike* spike16 = new Spike();
	gameObjectsToBeRendered.push_back(spike16);
	Spike* spike17 = new Spike();
	gameObjectsToBeRendered.push_back(spike17);
	Spike* spike18 = new Spike();
	gameObjectsToBeRendered.push_back(spike18);
	Spike* spike19 = new Spike();
	gameObjectsToBeRendered.push_back(spike19);
	Spike* spike20 = new Spike();
	gameObjectsToBeRendered.push_back(spike20);

	Checkpoint* checkpoint1 = new Checkpoint();
	gameObjectsToBeRendered.push_back(checkpoint1);
	Checkpoint* checkpoint2 = new Checkpoint();
	gameObjectsToBeRendered.push_back(checkpoint2);
	Checkpoint* checkpoint3 = new Checkpoint();
	gameObjectsToBeRendered.push_back(checkpoint3);

	// Set the positions of the game objects
	floor1->SetPosition(Math::sVector(4.0f, -1.25f, 0.0f));
	floor2->SetPosition(Math::sVector(7.0f, 1.25f, 0.0f));
	floor3->SetPosition(Math::sVector(14.0f, -1.25f, 0.0f));
	floor4->SetPosition(Math::sVector(24.0f, -1.25f, 0.0f));

	spike1->SetPosition(Math::sVector(1.3f, -1.0f, 0.0f));
	spike2->SetPosition(Math::sVector(3.3f, 1.0f, 0.0f));
	spike3->SetPosition(Math::sVector(3.3f, -1.0f, 0.0f));
	spike4->SetPosition(Math::sVector(5.3f, -1.0f, 0.0f));
	spike5->SetPosition(Math::sVector(5.3f, 1.0f, 0.0f));
	spike6->SetPosition(Math::sVector(6.3f, -1.0f, 0.0f));
	spike7->SetPosition(Math::sVector(8.3f, -1.0f, 0.0f));
	spike8->SetPosition(Math::sVector(9.3f, -1.0f, 0.0f));
	spike9->SetPosition(Math::sVector(9.3f, 1.0f, 0.0f));
	spike10->SetPosition(Math::sVector(12.0f, -0.5f, 0.0f));
	spike11->SetPosition(Math::sVector(16.0f, -0.3f, 0.0f));
	spike12->SetPosition(Math::sVector(16.0f, 1.6f, 0.0f));
	spike13->SetPosition(Math::sVector(19.0f, -0.1f, 0.0f));
	spike14->SetPosition(Math::sVector(19.0f, 1.8f, 0.0f));
	spike15->SetPosition(Math::sVector(22.0f, -0.6f, 0.0f));
	spike16->SetPosition(Math::sVector(22.0f, 1.3f, 0.0f));
	spike17->SetPosition(Math::sVector(25.0f, -0.6f, 0.0f));
	spike18->SetPosition(Math::sVector(25.0f, 1.3f, 0.0f));
	spike19->SetPosition(Math::sVector(26.0f, -0.6f, 0.0f));
	spike20->SetPosition(Math::sVector(26.0f, 1.3f, 0.0f));

	checkpoint1->SetPosition(Math::sVector(11.0f, 0.0f, 0.0f));
	checkpoint2->SetPosition(Math::sVector(14.5f, 0.0f, 0.0f));
	checkpoint3->SetPosition(Math::sVector(23.5f, 0.0f, 0.0f));

	// Set the main camera
	mainCamera = new MainCamera();
	
	// Set the follow object of the main camera
	dynamic_cast<MainCamera*>(mainCamera)->followObject = player;
	dynamic_cast<Player*>(player)->mainCamera = dynamic_cast<MainCamera*>(mainCamera);

	backgroundMusic.AudioConstructor("data/Audio/IWBTGBGM.wav", "bgm", 1, true);
	backgroundMusic.SubmitAudioToBePlayed();
	backgroundMusic.Play();

	return Results::Success;
}

eae6320::cResult eae6320::SideScrollingPlatformer::CleanUp()
{
	eae6320::Logging::OutputMessage("Cleaning up Chris Yuan Zhong's EAE6320 Example Game");

	auto result = Results::Success;

	for (auto & gameObject : gameObjectsToBeRendered)
	{
		if (gameObject)
			delete gameObject;
	}

	if (mainCamera)
	{
		delete mainCamera;
	}

	return result;
}

void eae6320::SideScrollingPlatformer::UpdateSimulationBasedOnInput()
{
}

void eae6320::SideScrollingPlatformer::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	ChrisZ::Physics::Update(i_elapsedSecondCount_sinceLastUpdate);

	mainCamera->Update(i_elapsedSecondCount_sinceLastUpdate);

	// Update the game objects
	std::vector<eae6320::Assets::GameObject*>::iterator it = gameObjectsToBeRendered.begin();
	while (it != gameObjectsToBeRendered.end())
	{
		eae6320::Assets::GameObject* gameObject = *it;
		if (gameObject)
		{
			if (gameObject->IsValid())
				gameObject->Update(i_elapsedSecondCount_sinceLastUpdate);
			else
			{
				// if the game object is the player, set the pointers to nullptr
				if (player == gameObject)
				{
					dynamic_cast<MainCamera*>(mainCamera)->followObject = nullptr;
					player = nullptr;
				}

				// delete the game object
				delete gameObject;

				// remove the game object from the vector
				it = gameObjectsToBeRendered.erase(it); // erase the current element and update the iterator
				continue;
			}
		}

		it++; // increment the iterator
	}
}

void eae6320::SideScrollingPlatformer::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	Graphics::SetClearColor(0x3f82f7ff);

	std::vector<Graphics::MeshEffectLocationTrio> meshEffectLocationTrios;

	for (auto & gameObject : gameObjectsToBeRendered)
	{
		if (gameObject && gameObject->IsValid() && gameObject->GetMesh() && gameObject->GetEffect())
		{
			Graphics::MeshEffectLocationTrio meshEffectLocationTrio;
			meshEffectLocationTrio.mesh = gameObject->GetMesh();
			meshEffectLocationTrio.effect = gameObject->GetEffect();
			meshEffectLocationTrio.constantData_drawCall.g_transform_localToWorld =
				gameObject->GetLocalToWorldTransformPrediction(i_elapsedSecondCount_sinceLastSimulationUpdate);
			meshEffectLocationTrios.push_back(meshEffectLocationTrio);
		}
	}

	Graphics::SubmitMeshEffectLocationTrios(meshEffectLocationTrios);

	Graphics::SubmitCameraData(mainCamera->GetWorldToCameraTransformPrediction(i_elapsedSecondCount_sinceLastSimulationUpdate), mainCamera->GetCameraToProjectedTransform());

	//backgroundMusic.SubmitAudioToBePlayed();
}
