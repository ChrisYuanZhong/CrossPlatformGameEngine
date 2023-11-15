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
			Transform* m_transform = nullptr;
			ChrisZ::Physics::RigidBody* m_rigidBody = nullptr;
			ChrisZ::Physics::Collider* m_collider = nullptr;

			Graphics::Mesh* m_mesh = nullptr;
			Graphics::Effect* m_effect = nullptr;
		};
	}
}

inline eae6320::Math::sVector eae6320::Assets::GameObject::GetPosition() const
{
	return m_transform->GetPosition();
}

inline void eae6320::Assets::GameObject::SetPosition(const Math::sVector& i_position)
{
	m_transform->SetPosition(i_position);
}

inline eae6320::Math::cQuaternion eae6320::Assets::GameObject::GetOrientation() const
{
	return m_transform->GetOrientation();
}

inline void eae6320::Assets::GameObject::SetOrientation(const Math::cQuaternion& i_orientation)
{
	m_transform->SetOrientation(i_orientation);
}

inline ChrisZ::Physics::RigidBody* eae6320::Assets::GameObject::GetRigidBody()
{
	return m_rigidBody;
}

inline ChrisZ::Physics::Collider* eae6320::Assets::GameObject::GetCollider()
{
	return m_collider;
}

inline eae6320::Graphics::Mesh* eae6320::Assets::GameObject::GetMesh() const
{
	return m_mesh;
}

inline void eae6320::Assets::GameObject::SetMesh(Graphics::Mesh* i_mesh)
{
	m_mesh = i_mesh;
}

inline eae6320::Graphics::Effect* eae6320::Assets::GameObject::GetEffect() const
{
	return m_effect;
}

inline void eae6320::Assets::GameObject::SetEffect(Graphics::Effect* i_effect)
{
	m_effect = i_effect;
}

inline eae6320::Math::cMatrix_transformation eae6320::Assets::GameObject::GetLocalToWorldTransformPrediction(const float i_secondCountToExtrapolate) const
{
	if (m_rigidBody)
	{
		return m_rigidBody->PredictFutureTransform(i_secondCountToExtrapolate);
	}
	else
	{
		return eae6320::Math::cMatrix_transformation(this->GetOrientation(), this->GetPosition());
	}
}