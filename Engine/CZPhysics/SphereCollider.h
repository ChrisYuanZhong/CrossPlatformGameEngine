#pragma once

#include <Engine/CZPhysics/Collider.h>

namespace ChrisZ
{
	namespace Physics
	{
		class SphereCollider : public Collider
		{
		public:
			// A constructor that takes the center, radius and game object as parameters
			SphereCollider(eae6320::Math::sVector i_center, float i_radius, eae6320::Assets::GameObject* i_gameObject);

			// A method to check if this sphere collider intersects with another collider
			bool Intersects(Collider* other) override;

			float GetRadius() const;
			void SetRadius(float i_radius);

		private:
			float radius = 0.0f;
		};
	}
}