#pragma once

// Includes
//=========

#include <Engine/Physics/sRigidBodyState.h>

namespace eae6320
{
	namespace Assets
	{
		class Camera
		{
			Camera() = default;
			~Camera();

		private:
			Physics::sRigidBodyState m_rigidBodyState;
		};
	}
}