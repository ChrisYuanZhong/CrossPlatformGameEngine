#include "MainCamera.h"
#include "Player.h"

void MainCamera::Update(const float i_secondCountToIntegrate)
{
	// Follow object
	if (followObject)
	{
		// Follow object with spring arm
		eae6320::Math::sVector targetPosition = followObject->GetPosition() + eae6320::Math::sVector(0.0f, 0.5f, springArmLength);
		eae6320::Math::sVector direction = targetPosition - GetPosition();

		eae6320::Math::sVector relativeVelocity = followObject->GetRigidBody()->GetVelocity() - m_rigidBody->GetVelocity();
		eae6320::Math::sVector force = -springConstant * direction - springDamping * relativeVelocity;
		m_rigidBody->AddForce(-force);
	}
	else
	{
		// Freeze camera
		m_rigidBody->SetVelocity(eae6320::Math::sVector(0.0f, 0.0f, 0.0f));
	}
}

void MainCamera::Reset()
{
	SetPosition(dynamic_cast<Player*>(followObject)->GetLastCheckpointPosition() + eae6320::Math::sVector(0.0f, 0.0f, springArmLength));
	m_rigidBody->SetVelocity(eae6320::Math::sVector(0.0f, 0.0f, 0.0f));
}