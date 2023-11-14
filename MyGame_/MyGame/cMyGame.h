/*
	This class is your specific game
*/

#ifndef EAE6320_CMYGAME_H
#define EAE6320_CMYGAME_H

// Includes
//=========

#include <Engine/Application/iApplication.h>
#include <Engine/Results/Results.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/Assets/GameObject.h>
#include <Engine/Assets/Camera.h>

#if defined( EAE6320_PLATFORM_WINDOWS )
	#include "Resource Files/Resource.h"
#endif

// Class Declaration
//==================

namespace eae6320
{
	struct GameInputs
	{
		bool isWDown = false;
		bool isADown = false;
		bool isSDown = false;
		bool isDDown = false;
		bool is1Down = false;
		bool is2Down = false;

		// For Camera
		bool isUpArrowDown = false;
		bool isDownArrowDown = false;
		bool isLeftArrowDown = false;
		bool isRightArrowDown = false;

		void Reset()
		{
			isWDown = false;
			isADown = false;
			isSDown = false;
			isDDown = false;
			is1Down = false;
			is2Down = false;

			isUpArrowDown = false;
			isDownArrowDown = false;
			isLeftArrowDown = false;
			isRightArrowDown = false;
		}
	};

	class cMyGame final : public Application::iApplication
	{
		// Inherited Implementation
		//=========================

	private:

		// Configuration
		//--------------

#if defined( EAE6320_PLATFORM_WINDOWS )
		// The main window's name will be displayed as its caption (the text that is displayed in the title bar).
		// You can make it anything that you want, but please keep the platform name and debug configuration at the end
		// so that it's easy to tell at a glance what kind of build is running.
		const char* GetMainWindowName() const final
		{
			return "Chris Yuan Zhong's EAE6320 Example Game"
				" -- "
#if defined( EAE6320_PLATFORM_D3D )
				"Direct3D"
#elif defined( EAE6320_PLATFORM_GL )
				"OpenGL"
#endif
#ifdef _DEBUG
				" -- Debug"
#endif
			;
		}
		// Window classes are almost always identified by name;
		// there is a unique "ATOM" associated with them,
		// but in practice Windows expects to use the class name as an identifier.
		// If you don't change the name below
		// your program could conceivably have problems if it were run at the same time on the same computer
		// as one of your classmate's.
		// You don't need to worry about this for our class,
		// but if you ever ship a real project using this code as a base you should set this to something unique
		// (a generated GUID would be fine since this string is never seen)
		const char* GetMainWindowClassName() const final { return "Chris Yuan Zhong's EAE6320 Example Main Window Class"; }
		// The following three icons are provided:
		//	* IDI_EAEGAMEPAD
		//	* IDI_EAEALIEN
		//	* IDI_VSDEFAULT_LARGE / IDI_VSDEFAULT_SMALL
		// If you want to try creating your own a convenient website that will help is: http://icoconvert.com/
		const WORD* GetLargeIconId() const final { static constexpr WORD iconId_large = IDI_EAEALIEN; return &iconId_large; }
		const WORD* GetSmallIconId() const final { static constexpr WORD iconId_small = IDI_EAEGAMEPAD; return &iconId_small; }
#endif

		// Run
		//----

		void UpdateBasedOnInput() final;

		// Inputs
		//-------
		
		GameInputs gameInputs;

		// Initialize / Clean Up
		//----------------------

		cResult Initialize() final;
		cResult CleanUp() final;

		virtual void UpdateSimulationBasedOnInput() override;
		virtual void UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate) override;

		virtual void SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate) override;

		static constexpr unsigned int numGameObjectsToBeRendered = 3;

		static constexpr unsigned int numPairs = 3;
		//eae6320::Graphics::Mesh* meshes[numPairs]{}; // 0: original, 1: modified
		//eae6320::Graphics::Effect* effects[numPairs]{}; // 0: original, 1: modified
		eae6320::Assets::GameObject* gameObjectsToBeRendered[numGameObjectsToBeRendered];
		eae6320::Assets::Camera* mainCamera;

		// Camera Objects
		eae6320::Assets::Camera camera;

		// Gameplay Variables
		//-------------------

		float velocity = 0.2f;
		bool isMeshModified = false;
	};
}

// Result Definitions
//===================

namespace eae6320
{
	namespace Results
	{
		namespace Application
		{
			// You can add specific results for your game here:
			//	* The System should always be Application
			//	* The __LINE__ macro is used to make sure that every result has a unique ID.
			//		That means, however, that all results _must_ be defined in this single file
			//		or else you could have two different ones with equal IDs.
			//	* Note that you can define multiple Success codes.
			//		This can be used if the caller may want to know more about how a function succeeded.
			constexpr cResult ExampleResult( IsFailure, eSystem::Application, __LINE__, Severity::Default );
		}
	}
}

#endif	// EAE6320_CEXAMPLEGAME_H
