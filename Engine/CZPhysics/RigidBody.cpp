#include "RigidBody.h"

#include <Engine/CZPhysics/CZPhysics.h>
#include <Engine/Assets/GameObject.h>

#include <Engine/Logging/Logging.h>

ChrisZ::Physics::RigidBody::RigidBody(eae6320::Assets::GameObject* i_gameObject) : gameObject(i_gameObject)
{
	// Set default values
	velocity = eae6320::Math::sVector(0.0f, 0.0f, 0.0f);
	acceleration = eae6320::Math::sVector(0.0f, 0.0f, 0.0f);
	dragCoefficient = 2.0f;
	angularSpeed = 0.0f;
	angularVelocity_axis_local = eae6320::Math::sVector(0.0f, 1.0f, 0.0f);
	mass = 1.0f;

	// Add this rigid body to the physics system
	ChrisZ::Physics::AddRigidBody(this);
}

void ChrisZ::Physics::RigidBody::AddForce(const eae6320::Math::sVector i_force)
{
	// Calculate the change in velocity
	const auto deltaVelocity = i_force / mass;

	// Update velocity
	velocity += deltaVelocity;
}

void ChrisZ::Physics::RigidBody::AddForceAtLocation(const eae6320::Math::sVector i_force, const eae6320::Math::sVector i_pointOfImpact)
{
	// Calculate the change in velocity
	const auto deltaVelocity = i_force / mass;

	// Update velocity
	velocity += deltaVelocity;

	// Calculate the change in angular velocity
	const auto deltaAngularVelocity = eae6320::Math::Cross(i_pointOfImpact - gameObject->GetPosition(), i_force) / mass;

	// Update angular velocity
	angularVelocity_axis_local += deltaAngularVelocity;
}

void ChrisZ::Physics::RigidBody::AddImpulse(const eae6320::Math::sVector i_impulse)
{
	// Calculate the change in velocity
	const auto deltaVelocity = i_impulse / mass;

	// Update velocity
	velocity += deltaVelocity;
}

void ChrisZ::Physics::RigidBody::AddImpulseAtLocation(const eae6320::Math::sVector i_impulse, const eae6320::Math::sVector i_pointOfImpact)
{
	// Calculate the change in velocity
	const auto deltaVelocity = i_impulse / mass;

	// Update velocity
	velocity += deltaVelocity;

	// Calculate the change in angular velocity
	const auto deltaAngularVelocity = eae6320::Math::Cross(i_pointOfImpact - gameObject->GetPosition(), i_impulse) / mass;

	// Update angular velocity
	angularVelocity_axis_local += deltaAngularVelocity;
}

void ChrisZ::Physics::RigidBody::Update(const float i_secondCountToIntegrate)
{
	// Update position
	{
		gameObject->SetPosition(gameObject->GetPosition() + velocity * i_secondCountToIntegrate);

		// Update center of collider
		gameObject->GetCollider()->SetCenter(gameObject->GetPosition());
	}
	// Update velocity
	{
		velocity += acceleration * i_secondCountToIntegrate;
		velocity -= velocity * dragCoefficient * i_secondCountToIntegrate;
	}
	// Update orientation
	{
		const auto rotation = eae6320::Math::cQuaternion(angularSpeed * i_secondCountToIntegrate, angularVelocity_axis_local);
		gameObject->SetOrientation(rotation * gameObject->GetOrientation());
		gameObject->SetOrientation(gameObject->GetOrientation().GetNormalized());
	}
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

eae6320::Math::cMatrix_transformation ChrisZ::Physics::RigidBody::PredictFutureTransform(const float i_secondCountToExtrapolate) const
{
	return eae6320::Math::cMatrix_transformation(PredictFutureOrientation(i_secondCountToExtrapolate), PredictFuturePosition(i_secondCountToExtrapolate));
}
