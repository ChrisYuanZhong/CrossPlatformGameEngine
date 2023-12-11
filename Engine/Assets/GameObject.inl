#pragma once

#include "GameObject.h"

inline void eae6320::Assets::GameObject::Destroy(GameObject* gameObjectToDestroy)
{
	if (gameObjectToDestroy)
		gameObjectToDestroy->SetIsValid(false);
}

inline bool eae6320::Assets::GameObject::IsValid() const
{
	return m_isValid;
}

inline void eae6320::Assets::GameObject::SetIsValid(bool i_isValid)
{
	m_isValid = i_isValid;
}

inline eae6320::Math::sVector eae6320::Assets::GameObject::GetPosition() const
{
	return m_transform->GetPosition();
}

inline void eae6320::Assets::GameObject::SetPosition(const Math::sVector& i_position)
{
	m_transform->SetPosition(i_position);

	// Update the position of the collider
	if (m_collider)
	{
		m_collider->SetCenter(GetPosition() + m_collider->GetCenterOffset());
	}
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