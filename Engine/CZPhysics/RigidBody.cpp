#include "RigidBody.h"

#include <Engine/Assets/GameObject.h>

void ChrisZ::Physics::RigidBody::Update(const float i_secondCountToIntegrate)
{
	// Update position
	{
		gameObject->GetTransform().SetPosition(gameObject->GetTransform().GetPosition() + velocity * i_secondCountToIntegrate);
	}
	// Update velocity
	{
		velocity += acceleration * i_secondCountToIntegrate;
		velocity -= velocity * dragCoefficient * i_secondCountToIntegrate;
	}
	// Update orientation
	{
		const auto rotation = eae6320::Math::cQuaternion(angularSpeed * i_secondCountToIntegrate, angularVelocity_axis_local);
		gameObject->GetTransform().SetOrientation(rotation * gameObject->GetTransform().GetOrientation());
		gameObject->GetTransform().SetOrientation(gameObject->GetTransform().GetOrientation().GetNormalized());
	}
}

eae6320::Math::sVector ChrisZ::Physics::RigidBody::PredictFuturePosition(const float i_secondCountToExtrapolate) const
{
	return gameObject->GetTransform().GetPosition() + velocity * i_secondCountToExtrapolate;
}

eae6320::Math::cQuaternion ChrisZ::Physics::RigidBody::PredictFutureOrientation(const float i_secondCountToExtrapolate) const
{
	const auto rotation = eae6320::Math::cQuaternion(angularSpeed * i_secondCountToExtrapolate, angularVelocity_axis_local);
	return eae6320::Math::cQuaternion(rotation * gameObject->GetTransform().GetOrientation()).GetNormalized();
}

eae6320::Math::cMatrix_transformation ChrisZ::Physics::RigidBody::PredictFutureTransform(const float i_secondCountToExtrapolate) const
{
	return eae6320::Math::cMatrix_transformation(PredictFutureOrientation(i_secondCountToExtrapolate), PredictFuturePosition(i_secondCountToExtrapolate));
}
