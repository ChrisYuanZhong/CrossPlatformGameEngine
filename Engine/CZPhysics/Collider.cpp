#include "Collider.h"

eae6320::Assets::GameObject* ChrisZ::Physics::Collider::GetGameObject()
{
	return this->gameObject;
}

void ChrisZ::Physics::Collider::SetCenter(eae6320::Math::sVector center)
{
	this->center = center;
}

eae6320::Math::sVector ChrisZ::Physics::Collider::GetCenter() const
{
	return this->center;
}
