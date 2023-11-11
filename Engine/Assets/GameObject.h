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
			
			inline Transform& GetTransform() { return m_transform; }
			
			inline ChrisZ::Physics::RigidBody* GetRigidBody() { return &m_rigidBody; }

			inline ChrisZ::Physics::SphereCollider& GetSphereCollider() { return m_sphereCollider; }

			inline Graphics::Mesh* GetMesh() const { return m_mesh; }
			inline void SetMesh(Graphics::Mesh* i_mesh) { m_mesh = i_mesh; }
			inline Graphics::Effect* GetEffect() const { return m_effect; }
			inline void SetEffect(Graphics::Effect* i_effect) { m_effect = i_effect; }

			//inline void Update(const float i_secondCountToIntegrate) { m_rigidBody.Update(i_secondCountToIntegrate); }
			inline Math::cMatrix_transformation GetLocalToWorldTransformPrediction(const float i_secondCountToExtrapolate) const { return m_rigidBody.PredictFutureTransform(i_secondCountToExtrapolate); }

		private:
			Transform m_transform;
			ChrisZ::Physics::RigidBody m_rigidBody = ChrisZ::Physics::RigidBody(this);
			ChrisZ::Physics::SphereCollider m_sphereCollider = ChrisZ::Physics::SphereCollider(Math::sVector(0.0f, 0.0f, 0.0f), 0.1f, this);

			Graphics::Mesh* m_mesh;
			Graphics::Effect* m_effect;
		};
	}
}