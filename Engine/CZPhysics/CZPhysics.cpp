#include "CZPhysics.h"

#include <Engine/Assets/GameObject.h>
//#include <Engine/Assets/GameObject-inl.h>
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

                        // Add colliding colliders to each other's lists if not already added
                        if (!collider->IsCollidingWith(other))
                        {
                            collider->AddCollidingCollider(other);
                            other->AddCollidingCollider(collider);
                        }

                        // Notify GameObjects about the collision
                        collider->GetGameObject()->OnCollisionStay(other);
                        other->GetGameObject()->OnCollisionStay(collider);

                        // Handle collision if the two colliders are not triggers
                        if (!(collider->IsTrigger() || other->IsTrigger()))
                        {
                            HandleCollision(collider, other, collisionInfo);
                        }
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
    // Get the rigid bodies of the colliding colliders
    RigidBody* bodyA = collider->GetGameObject()->GetRigidBody();
    RigidBody* bodyB = other->GetGameObject()->GetRigidBody();

    eae6320::Math::sVector velocityA = eae6320::Math::sVector(0.0f, 0.0f, 0.0f);
    float massA = FLT_MAX;
    if (bodyA)
    {
        velocityA = bodyA->GetVelocity();
        massA = bodyA->GetMass();
    }

	eae6320::Math::sVector velocityB = eae6320::Math::sVector(0.0f, 0.0f, 0.0f);
	float massB = FLT_MAX;
	if (bodyB)
	{
		velocityB = bodyB->GetVelocity();
		massB = bodyB->GetMass();
	}

    // Calculate the relative velocity of the two bodies along the collision normal
    eae6320::Math::sVector relativeVelocity = velocityB - velocityA;
    float velocityAlongNormal = Dot(relativeVelocity, collisionInfo.contactNormal);

    // Do nothing if the bodies are moving away from each other
    if (velocityAlongNormal > 0.0f)
    {
        return;
    }

    // Calculate the restitution coefficient based on the bounciness of the two bodies
    float e = std::min(collider->GetRestitution(), other->GetRestitution());

    // Calculate the impulse scalar
    float j = -(1 + e) * velocityAlongNormal;
    j /= 1 / massA + 1 / massB;

    // Apply the impulse force to the bodies
    eae6320::Math::sVector impulse = j * collisionInfo.contactNormal;
    // Check if bodyA has a rigid body
    if (bodyA)
    {
        bodyA->AddImpulse(-impulse);
    }
    // Check if bodyB has a rigid body
    if (bodyB)
    {
        bodyB->AddImpulse(impulse);
    }

    // Calculate the amount of position correction needed to prevent interpenetration
    // A small percentage of the penetration depth is used to avoid jittering
    const float percent = 0.2f;
    // A small value is added to the penetration depth to avoid division by zero
    const float slop = 0.01f;
    eae6320::Math::sVector correction = (std::max(collisionInfo.penetrationDepth - slop, 0.0f) / (1 / massA + 1 / massB)) * percent * collisionInfo.contactNormal;

    // Correct the positions of the bodies
    // Check if bodyA has a rigid body
    if (bodyA)
    {
        bodyA->GetGameObject()->SetPosition(bodyA->GetGameObject()->GetPosition() - (1 / massA) * correction);
    }
    // Check if bodyB has a rigid body
    if (bodyB)
    {
        bodyB->GetGameObject()->SetPosition(bodyB->GetGameObject()->GetPosition() + (1 / massB) * correction);
    }
}