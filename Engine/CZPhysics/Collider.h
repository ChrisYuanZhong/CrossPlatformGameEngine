#pragma once

// Includes
//=========

#include <Engine/Math/cQuaternion.h>
#include <Engine/Math/sVector.h>

#include <unordered_set>

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
		struct CollisionInfo;
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
			// Constructor
			Collider(eae6320::Assets::GameObject* i_gameObject);

			// A virtual method to check if this collider intersects with another collider
			virtual CollisionInfo Intersects(Collider* other) = 0;

			// A method to get the game object this collider is attached to
			eae6320::Assets::GameObject* GetGameObject();

			// A method to check if this collider can overlap with other colliders
			bool IsTrigger() const;

			// A method to get the center of the collider
			eae6320::Math::sVector GetCenter() const;

			// A method to set the center of the collider
			void SetCenter(eae6320::Math::sVector center);

			// A method to get the energy consumed after collision
			float GetRestitution() const;

			// Method to check if a collider is colliding with self
			bool IsCollidingWith(Collider* other) const;

			// Method to add a collider to the hash set of colliding colliders
			void AddCollidingCollider(Collider* other);

			// Method to remove a collider from the hash set of colliding colliders
			void RemoveCollidingCollider(Collider* other);

		protected:
			// The center of the collider
			eae6320::Math::sVector center = eae6320::Math::sVector(0.0f, 0.0f, 0.0f);

			// The energy consumed after collision
			float restitution = 0.5f;

			// Hash set of pointers to colliders that are colliding with self
			std::unordered_set<Collider*> collidingColliders;

			// Whether this collider can overlap with other colliders
			bool isTrigger = false;

			// The game object that this collider is attached to
			eae6320::Assets::GameObject* gameObject;
		};
	}
}