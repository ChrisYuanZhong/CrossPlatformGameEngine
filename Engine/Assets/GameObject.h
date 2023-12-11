#pragma once

// Includes
//=========

#include "Transform.h"

#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Graphics/Mesh.h>
#include <Engine/Graphics/Effect.h>

#include <Engine/CZPhysics/CZPhysics.h>

namespace eae6320
{
	namespace Assets
	{
		class GameObject
		{
		public:
			GameObject();
			GameObject(Graphics::Mesh* i_mesh, Graphics::Effect* i_effect);
			virtual ~GameObject();
			inline void Destroy(GameObject* gameObjectToDestroy);
			
			// Virtual collision callback methods
			virtual void OnCollisionEnter(ChrisZ::Physics::Collider* other) {}
			virtual void OnCollisionExit(ChrisZ::Physics::Collider* other) {}
			virtual void OnCollisionStay(ChrisZ::Physics::Collider* other) {}

			// Virtual update method
			virtual void Update(const float i_secondCountToIntegrate) {}

			// Getters and setters
			inline bool IsValid() const;
			inline void SetIsValid(const bool i_isValid);
			inline Math::sVector GetPosition() const;
			inline void SetPosition(const Math::sVector& i_position);
			inline Math::cQuaternion GetOrientation() const;
			inline void SetOrientation(const Math::cQuaternion& i_orientation);
			
			inline ChrisZ::Physics::RigidBody* GetRigidBody();

			inline ChrisZ::Physics::Collider* GetCollider();

			inline Graphics::Mesh* GetMesh() const;
			inline void SetMesh(Graphics::Mesh* i_mesh);
			inline Graphics::Effect* GetEffect() const;
			inline void SetEffect(Graphics::Effect* i_effect);

			inline Math::cMatrix_transformation GetLocalToWorldTransformPrediction(const float i_secondCountToExtrapolate) const;

		protected:
			bool m_isValid = false;
			Transform* m_transform = nullptr;
			ChrisZ::Physics::RigidBody* m_rigidBody = nullptr;
			ChrisZ::Physics::Collider* m_collider = nullptr;

			Graphics::Mesh* m_mesh = nullptr;
			Graphics::Effect* m_effect = nullptr;
		};
	}
}

#include "GameObject.inl"