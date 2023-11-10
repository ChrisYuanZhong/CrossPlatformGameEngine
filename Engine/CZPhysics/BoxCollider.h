#pragma once

#include <Engine/CZPhysics/Collider.h>

namespace ChrisZ
{
	namespace Physics
	{
		class BoxCollider : public Collider
		{
		public:
			// A constructor that takes the center, size, orientation and game object as parameters
			BoxCollider(eae6320::Math::sVector i_center, eae6320::Math::sVector i_extents, eae6320::Math::cQuaternion i_orientation, eae6320::Assets::GameObject* i_gameObject);

			// A method to check if this box collider intersects with another collider
			bool Intersects(Collider* other) override;

			// A method to get the closest point on the box to a given point
			eae6320::Math::sVector ClosestPoint(eae6320::Math::sVector point);

		private:
			// The size and orientation of the box
			eae6320::Math::sVector extents = eae6320::Math::sVector(0.0f, 0.0f, 0.0f);
			eae6320::Math::cQuaternion orientation;
		};
	}
}