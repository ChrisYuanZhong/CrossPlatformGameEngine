#include "RigidBody.h"

#include <Engine/CZPhysics/CZPhysics.h>
#include <Engine/Assets/GameObject.h>

#include <Engine/Logging/Logging.h>

ChrisZ::Physics::RigidBody::RigidBody(eae6320::Assets::GameObject* i_gameObject) : gameObject(i_gameObject)
{
	// Set default values
	velocity = eae6320::Math::sVector(0.0f, 0.0f, 0.0f);
	acceleration = eae6320::Math::sVector(0.0f, 0.0f, 0.0f);
	dragCoefficient = 0.6f;
	angularSpeed = 0.0f;
	angularVelocity_axis_local = eae6320::Math::sVector(0.0f, 1.0f, 0.0f);
	mass = 1.0f;
	gravityEnabled = false;
	g = 9.8f;

	// Add this rigid body to the physics system
	ChrisZ::Physics::AddRigidBody(this);
}

void ChrisZ::Physics::RigidBody::Update(const float i_secondCountToIntegrate)
{
	// Apply gravity if enabled
	if (gravityEnabled)
	{
		force.y -= mass * g;
	}
	// Update acceleration
	{
		acceleration = force / mass;
	}
	// Update position
	{
		gameObject->SetPosition(gameObject->GetPosition() + velocity * i_secondCountToIntegrate);

		// Update center of collider
		gameObject->GetCollider()->SetCenter(gameObject->GetPosition());
	}
	// Update velocity
	{
		velocity *= (1 - dragCoefficient * i_secondCountToIntegrate);
		velocity += acceleration * i_secondCountToIntegrate;
	}
	// Update orientation
	{
		const auto rotation = eae6320::Math::cQuaternion(angularSpeed * i_secondCountToIntegrate, angularVelocity_axis_local);
		gameObject->SetOrientation(rotation * gameObject->GetOrientation());
		gameObject->SetOrientation(gameObject->GetOrientation().GetNormalized());
	}

	// Reset force
	force = eae6320::Math::sVector(0.0f, 0.0f, 0.0f);
}

eae6320::Assets::GameObject* ChrisZ::Physics::RigidBody::GetGameObject() const
{
	return gameObject;
}

void ChrisZ::Physics::RigidBody::AddForceAtLocation(const eae6320::Math::sVector i_force, const eae6320::Math::sVector i_pointOfImpact)
{
	// Update velocity
	velocity += i_force / mass;

	// Calculate the change in angular velocity
	const auto deltaAngularVelocity = eae6320::Math::Cross(i_pointOfImpact - gameObject->GetPosition(), i_force) / mass;

	// Update angular velocity
	angularVelocity_axis_local += deltaAngularVelocity;
}

void ChrisZ::Physics::RigidBody::AddImpulseAtLocation(const eae6320::Math::sVector i_impulse, const eae6320::Math::sVector i_pointOfImpact)
{
	// Update velocity
	velocity += i_impulse / mass;

	// Calculate the change in angular velocity
	const auto deltaAngularVelocity = eae6320::Math::Cross(i_pointOfImpact - gameObject->GetPosition(), i_impulse) / mass;

	// Update angular velocity
	angularVelocity_axis_local += deltaAngularVelocity;
}

eae6320::Math::sVector ChrisZ::Physics::RigidBody::PredictFuturePosition(const float i_secondCountToExtrapolate) const
{
	return gameObject->GetPosition() + velocity * i_secondCountToExtrapolate;
}

eae6320::Math::cQuaternion ChrisZ::Physics::RigidBody::PredictFutureOrientation(const float i_secondCountToExtrapolate) const
{
	const auto rotation = eae6320::Math::cQuaternion(angularSpeed * i_secondCountToExtrapolate, angularVelocity_axis_local);
	return eae6320::Math::cQuaternion(rotation * gameObject->GetOrientation()).GetNormalized();
}