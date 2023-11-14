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
			BoxCollider(eae6320::Math::sVector i_center, eae6320::Math::sVector i_extents, eae6320::Assets::GameObject* i_gameObject);

			// A method to check if this box collider intersects with another collider
			CollisionInfo Intersects(Collider* other) override;

			// Helper Methods
			//===============

			// A method to get the closest point on the box to a given point
			eae6320::Math::sVector ClosestPoint(eae6320::Math::sVector point);

			// Define a helper method to project the box onto an axis
			void ProjectOntoAxis(eae6320::Math::sVector axis, float& min, float& max);

			// Use the bounding sphere detection as a broad phase detection to check if this box collider has a possibility of intersecting with another box collider
			// Complexity: O(1)
			bool BoundingSphereDetection(BoxCollider* other);

			// Use the AABB method as a middle phase detection to check if this box collider has a possibility of intersecting with another box collider
			// Complexity: O(n)
			bool AABBDetection(BoxCollider* other);

			// Use the SAT method to check if this box collider intersects with another box collider
			// Complexity: O(n ^ 2)
			CollisionInfo SATDetection(BoxCollider* other);

		private:
			eae6320::Math::sVector extents = eae6320::Math::sVector(0.5f, 0.5f, 0.5f); // The half-size of the box along each axis
		};
	}
}