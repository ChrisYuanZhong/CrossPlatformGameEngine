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
			~GameObject();
			
			// Virtual collision callback methods
			virtual void OnCollisionEnter(ChrisZ::Physics::Collider* other) {}
			virtual void OnCollisionExit(ChrisZ::Physics::Collider* other) {}
			virtual void OnCollisionStay(ChrisZ::Physics::Collider* other) {}

			// Virtual update method
			virtual void Update(const float i_secondCountToIntegrate) {}

			//inline Transform* GetTransform() { return m_transform; }

			inline Math::sVector GetPosition() const { return m_transform->GetPosition(); }
			inline void SetPosition(const Math::sVector& i_position) { m_transform->SetPosition(i_position); }
			inline Math::cQuaternion GetOrientation() const { return m_transform->GetOrientation(); }
			inline void SetOrientation(const Math::cQuaternion& i_orientation) { m_transform->SetOrientation(i_orientation); }
			
			inline ChrisZ::Physics::RigidBody* GetRigidBody() { return m_rigidBody; }

			inline ChrisZ::Physics::Collider* GetCollider() { return m_collider; }

			inline Graphics::Mesh* GetMesh() const { return m_mesh; }
			inline void SetMesh(Graphics::Mesh* i_mesh) { m_mesh = i_mesh; }
			inline Graphics::Effect* GetEffect() const { return m_effect; }
			inline void SetEffect(Graphics::Effect* i_effect) { m_effect = i_effect; }

			inline Math::cMatrix_transformation GetLocalToWorldTransformPrediction(const float i_secondCountToExtrapolate) const { return m_rigidBody->PredictFutureTransform(i_secondCountToExtrapolate); }

		protected:
			Transform* m_transform;
			ChrisZ::Physics::RigidBody* m_rigidBody;
			ChrisZ::Physics::Collider* m_collider;

			Graphics::Mesh* m_mesh;
			Graphics::Effect* m_effect;
		};
	}
}