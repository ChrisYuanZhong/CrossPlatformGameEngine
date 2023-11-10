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

			//// Log the position of the colliders
			//{
			//	std::string message = "Collider position: ";
			//	message += std::to_string(rigidBody->GetGameObject()->GetSphereCollider().GetCenter().x) + ", ";
			//	message += std::to_string(rigidBody->GetGameObject()->GetSphereCollider().GetCenter().y) + ", ";
			//	message += std::to_string(rigidBody->GetGameObject()->GetSphereCollider().GetCenter().z);
			//	eae6320::Logging::OutputMessage(message.c_str());
			//}
		}
	}
	// Check for collisions
	{
		for (auto collider : colliders)
		{
			for (auto other : colliders)
			{
				if (collider != other)
				{
					if (collider->Intersects(other))
					{
						// Log that there is a collision
						eae6320::Logging::OutputMessage("Collision detected!");

						//collider->GetGameObject()->OnCollision(other->GetGameObject());

						//// Get the rigid bodies
						//RigidBody* rigidBody1 = collider1->GetGameObject()->GetRigidBody();
						//RigidBody* rigidBody2 = collider2->GetGameObject()->GetRigidBody();

						//// Apply some physics response, such as applying forces, impulses, or changing velocities
						//// You can use your own physics logic here, or use a library like Bullet Physics
						//// For example, you can use the conservation of momentum formula to calculate the new velocities
						//// v1' = (m1 - m2) / (m1 + m2) * v1 + 2 * m2 / (m1 + m2) * v2
						//// v2' = 2 * m1 / (m1 + m2) * v1 + (m2 - m1) / (m1 + m2) * v2
						//// Where v1 and v2 are the initial velocities, and v1' and v2' are the final velocities
						//// And m1 and m2 are the masses of the rigid bodies
						//Vector3 v1 = rigidBody1->GetVelocity();
						//Vector3 v2 = rigidBody2->GetVelocity();
						//float m1 = rigidBody1->GetMass();
						//float m2 = rigidBody2->GetMass();
						//Vector3 v1Prime = (m1 - m2) / (m1 + m2) * v1 + 2 * m2 / (m1 + m2) * v2;
						//Vector3 v2Prime = 2 * m1 / (m1 + m2) * v1 + (m2 - m1) / (m1 + m2) * v2;
						//rigidBody1->SetVelocity(v1Prime);
						//rigidBody2->SetVelocity(v2Prime);
					}
				}
			}
		}
	}
}
