#include "Collider.h"

#include <Engine/CZPhysics/CZPhysics.h>
#include <Engine/Assets/GameObject.h>

ChrisZ::Physics::Collider::Collider(eae6320::Math::sVector i_center, eae6320::Assets::GameObject* i_gameObject) : gameObject(i_gameObject)
{
	center = i_center;

	// Add this collider to the physics system
	ChrisZ::Physics::AddCollider(this);
}

eae6320::Assets::GameObject* ChrisZ::Physics::Collider::GetGameObject()
{
	return this->gameObject;
}

bool ChrisZ::Physics::Collider::IsTrigger() const
{
	return this->isTrigger;
}

eae6320::Math::sVector ChrisZ::Physics::Collider::GetCenter() const
{
	return this->center;
}

void ChrisZ::Physics::Collider::SetCenter(eae6320::Math::sVector center)
{
	this->center = center;
}

float ChrisZ::Physics::Collider::GetRestitution() const
{
	return this->restitution;
}

void ChrisZ::Physics::Collider::SetRestitution(float restitution)
{
	this->restitution = restitution;
}

float ChrisZ::Physics::Collider::GetFrictionCoefficient() const
{
	return this->frictionCoefficient;
}

void ChrisZ::Physics::Collider::SetFrictionCoefficient(float frictionCoefficient)
{
	this->frictionCoefficient = frictionCoefficient;
}

bool ChrisZ::Physics::Collider::IsCollidingWith(Collider* other) const
{
	// Check if the other collider exists in the hash set
	return collidingColliders.find(other) != collidingColliders.end();
}

void ChrisZ::Physics::Collider::AddCollidingCollider(Collider* other)
{
	collidingColliders.insert(other);

	// Call collision enter
	gameObject->OnCollisionEnter(other);
	other->GetGameObject()->OnCollisionEnter(this);
}

void ChrisZ::Physics::Collider::RemoveCollidingCollider(Collider* other)
{
	collidingColliders.erase(other);

	// Call collision exit
	gameObject->OnCollisionExit(other);
	other->GetGameObject()->OnCollisionExit(this);
}
