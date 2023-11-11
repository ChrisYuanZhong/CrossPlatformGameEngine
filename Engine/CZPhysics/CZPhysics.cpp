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
					if (collider->Intersects(other))
					{
						// Log that there is a collision
						eae6320::Logging::OutputMessage("Collision detected!");

						//collider->GetGameObject()->OnCollision(other->GetGameObject());
						//other->GetGameObject()->OnCollision(collider->GetGameObject());

						// Get the rigid bodies
						RigidBody* rigidBody1 = collider->GetGameObject()->GetRigidBody();
						RigidBody* rigidBody2 = other->GetGameObject()->GetRigidBody();

						// Apply some physics response
						eae6320::Math::sVector v1 = rigidBody1->GetVelocity();
						eae6320::Math::sVector v2 = rigidBody2->GetVelocity();
						float m1 = rigidBody1->GetMass();
						float m2 = rigidBody2->GetMass();
						eae6320::Math::sVector v1Prime = (m1 - m2) / (m1 + m2) * v1 + 2 * m2 / (m1 + m2) * v2;
						eae6320::Math::sVector v2Prime = 2 * m1 / (m1 + m2) * v1 + (m2 - m1) / (m1 + m2) * v2;
						rigidBody1->SetVelocity(v1Prime);
						rigidBody2->SetVelocity(v2Prime);
					}
				}
			}
		}
	}
}
