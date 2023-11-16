#pragma once

#ifndef CZPHYSICS_COLLIDER_INL
#define CZPHYSICS_COLLIDER_INL

#include "Collider.h"

#endif // !CZPHYSICS_COLLIDER_INL

inline bool ChrisZ::Physics::Collider::IsTrigger() const
{
	return this->isTrigger;
}

inline void ChrisZ::Physics::Collider::SetIsTrigger(bool isTrigger)
{
	this->isTrigger = isTrigger;
}

inline eae6320::Math::sVector ChrisZ::Physics::Collider::GetCenter() const
{
	return this->center;
}

inline void ChrisZ::Physics::Collider::SetCenter(eae6320::Math::sVector center)
{
	this->center = center;
}

inline float ChrisZ::Physics::Collider::GetRestitution() const
{
	return this->restitution;
}

inline void ChrisZ::Physics::Collider::SetRestitution(float restitution)
{
	this->restitution = restitution;
}

inline float ChrisZ::Physics::Collider::GetFrictionCoefficient() const
{
	return this->frictionCoefficient;
}

inline void ChrisZ::Physics::Collider::SetFrictionCoefficient(float frictionCoefficient)
{
	this->frictionCoefficient = frictionCoefficient;
}

inline bool ChrisZ::Physics::Collider::IsCollidingWith(Collider* other) const
{
	// Check if the other collider exists in the hash set
	return collidingColliders.find(other) != collidingColliders.end();
}