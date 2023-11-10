#pragma once

#include <Engine/Math/cQuaternion.h>
#include <Engine/Math/sVector.h>

// Forward Declarations
//=====================

namespace eae6320
{
	namespace Math
	{
		class cMatrix_transformation;
	}

	namespace Assets
	{
		class GameObject;
	}
}

namespace ChrisZ
{
	namespace Physics
	{
		// A base class for colliders
		class Collider
		{
		public:
			// A virtual method to check if this collider intersects with another collider
			virtual bool Intersects(Collider* other) = 0;

			// A method to get the game object this collider is attached to
			eae6320::Assets::GameObject* GetGameObject();

			// A method to set the center of the collider
			void SetCenter(eae6320::Math::sVector center);

			// A method to get the center of the collider
			eae6320::Math::sVector GetCenter() const;

		protected:
			// The center of the collider
			eae6320::Math::sVector center = eae6320::Math::sVector(0.0f, 0.0f, 0.0f);

			// The game object that this collider is attached to
			eae6320::Assets::GameObject* gameObject;
		};
	}
}