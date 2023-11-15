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