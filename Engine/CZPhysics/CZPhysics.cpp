#include "CZPhysics.h"

#include <Engine/Assets/GameObject.h>
#include <Engine/CZPhysics/SphereCollider.h>
#include <Engine/CZPhysics/BoxCollider.h>

#include <vector>

#include <Engine/Logging/Logging.h>

// A list of all rigid bodies
std::vector<ChrisZ::Physics::RigidBody*> rigidBodies;
// A list of all colliders
std::vector<ChrisZ::Physics::Collider*> colliders;

void ChrisZ::Physics::AddRigidBody(RigidBody* i_rigidBody)
{
	rigidBodies.push_back(i_rigidBody);
}

void ChrisZ::Physics::AddCollider(Collider* i_collider)
{
	colliders.push_back(i_collider);
}

void ChrisZ::Physics::Update(const float i_secondCountToIntegrate)
{
	// Update rigid bodies
	{
		for (auto rigidBody : rigidBodies)
		{
			rigidBody->Update(i_secondCountToIntegrate);
		}
	}

	// Check for collisions
	{
		// Iterate the colliders with an iterator
		for (size_t i = 0; i < colliders.size() - 1; ++i)
		{
			for (size_t j = i + 1; j < colliders.size(); ++j)
			{
				Collider* collider = colliders[i];
				Collider* other = colliders[j];

                if (collider != other)
                {
                    // Call Intersects to check for collision and get CollisionInfo
                    CollisionInfo collisionInfo = collider->Intersects(other);

                    // Check if there's a collision based on penetration depth
                    if (collisionInfo.penetrationDepth > 0.0f)
                    {
                        // Log that there is a collision
                        eae6320::Logging::OutputMessage("Collision detected!");

                        // Handle collision if the two colliders are not triggers
                        if (!(collider->IsTrigger() || other->IsTrigger()))
                        {
                            HandleCollision(collider, other, collisionInfo);
                        }

                        // Add colliding colliders to each other's lists if not already added
                        if (!collider->IsCollidingWith(other))
                        {
                            collider->AddCollidingCollider(other);
                            other->AddCollidingCollider(collider);
                        }

                        // Notify GameObjects about the collision
                        collider->GetGameObject()->OnCollisionStay(other);
                        other->GetGameObject()->OnCollisionStay(collider);
                    }
                    else
                    {
                        // If no collision, remove colliders from each other's lists if they are present
                        if (collider->IsCollidingWith(other))
                        {
                            collider->RemoveCollidingCollider(other);
                            other->RemoveCollidingCollider(collider);
                        }
                    }
                }
			}
		}
	}
}

void ChrisZ::Physics::HandleCollision(Collider* collider, Collider* other, CollisionInfo collisionInfo)
{

    // Calculate the relative velocity of the two colliding objects
    const eae6320::Math::sVector relativeVelocity = other->GetGameObject()->GetRigidBody()->GetVelocity() - collider->GetGameObject()->GetRigidBody()->GetVelocity();

    // Calculate the relative velocity in the normal direction of the collision
    const float relativeVelocityInNormalDirection = eae6320::Math::Dot(relativeVelocity, collisionInfo.contactNormal);

    // Check if the objects are moving towards each other
    if (relativeVelocityInNormalDirection < 0.0f)
    {
        // Calculate the restitution coefficient
        const float restitution = std::min(collider->GetRestitution(), other->GetRestitution());

        // Calculate the impulse magnitude
        const float impulseMagnitude = -(1.0f + restitution) * relativeVelocityInNormalDirection / (1.0f / collider->GetGameObject()->GetRigidBody()->GetMass() + 1.0f / other->GetGameObject()->GetRigidBody()->GetMass());

        // Calculate the impulse vector
        const eae6320::Math::sVector impulse = impulseMagnitude * collisionInfo.contactNormal;

        // Apply the impulse to the colliding objects
        collider->GetGameObject()->GetRigidBody()->AddImpulse(-impulse);
        other->GetGameObject()->GetRigidBody()->AddImpulse(impulse);
    }

    //// Get the rigid body of the collider
    //RigidBody* rigidBody = collider->GetGameObject()->GetGameObject()->GetRigidBody();

    //// If the rigid body exists, apply impulse
    //if (rigidBody)
    //{
    //    // Get the velocity of the rigid body
    //    eae6320::Math::sVector velocity = rigidBody->GetVelocity();

    //    // Calculate the impulse
    //    float impulse = eae6320::Math::Dot(velocity, collisionInfo.contactNormal);

    //    // Apply the impulse
    //    rigidBody->SetVelocity(velocity - impulse * collisionInfo.contactNormal);
    //}

    //// Get the rigid body of the other collider
    //RigidBody* otherRigidBody = other->GetGameObject()->GetGameObject()->GetRigidBody();

    //// If the rigid body exists, apply impulse
    //if (otherRigidBody)
    //{
    //    // Get the velocity of the rigid body
    //    eae6320::Math::sVector velocity = otherRigidBody->GetVelocity();

    //    // Calculate the impulse
    //    float impulse = eae6320::Math::Dot(velocity, collisionInfo.contactNormal);

    //    // Apply the impulse
    //    otherRigidBody->SetVelocity(velocity - impulse * collisionInfo.contactNormal);
    //}
}
