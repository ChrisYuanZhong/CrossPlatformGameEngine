#pragma once

// Includes
//=========

#include "Transform.h"

#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Graphics/Mesh.h>
#include <Engine/Graphics/Effect.h>
#include <Engine/CZPhysics/RigidBody.h>

namespace eae6320
{
	namespace Assets
	{
		class GameObject
		{
		public:
			GameObject() = default;
			GameObject(Graphics::Mesh* i_mesh, Graphics::Effect* i_effect);
			~GameObject();
			
			inline Transform GetTransform() const { return m_transform; }
			inline void SetTransform(Transform i_transform) { m_transform = i_transform; }
			inline ChrisZ::Physics::RigidBody GetRigidBody() const { return m_rigidBody; }
			inline Graphics::Mesh* GetMesh() const { return m_mesh; }
			inline void SetMesh(Graphics::Mesh* i_mesh) { m_mesh = i_mesh; }
			inline Graphics::Effect* GetEffect() const { return m_effect; }
			inline void SetEffect(Graphics::Effect* i_effect) { m_effect = i_effect; }

			//// Rigid Body State
			//inline Math::sVector GetPosition() const { return m_rigidBodyState.position; }
			//inline void SetPosition(Math::sVector i_position) { m_rigidBodyState.position = i_position; }
			//inline Math::cQuaternion GetOrientation() const { return m_rigidBodyState.orientation; }
			//inline void SetOrientation(Math::cQuaternion i_orientation) { m_rigidBodyState.orientation = i_orientation; }
			//inline Math::sVector GetVelocity() const { return m_rigidBodyState.velocity; }
			//inline void SetVelocity(Math::sVector i_velocity) { m_rigidBodyState.velocity = i_velocity; }
			//inline Math::sVector GetAcceleration() const { return m_rigidBodyState.acceleration; }
			//inline void SetAcceleration(Math::sVector i_acceleration) { m_rigidBodyState.acceleration = i_acceleration; }

			//inline Math::cMatrix_transformation GetLocalToWorldTransformPrediction(const float i_secondCountToExtrapolate) const { return m_rigidBodyState.PredictFutureTransform(i_secondCountToExtrapolate); }

			//inline void Update(const float i_secondCountToIntegrate) { m_rigidBodyState.Update(i_secondCountToIntegrate); }

			inline void Update(const float i_secondCountToIntegrate) { m_rigidBody.Update(i_secondCountToIntegrate); }
			inline Math::cMatrix_transformation GetLocalToWorldTransformPrediction(const float i_secondCountToExtrapolate) const { return m_rigidBody.PredictFutureTransform(i_secondCountToExtrapolate); }

		private:
			Transform m_transform;
			ChrisZ::Physics::RigidBody m_rigidBody;

			Graphics::Mesh* m_mesh;
			Graphics::Effect* m_effect;
			//Physics::sRigidBodyState m_rigidBodyState;
		};
	}
}