// Includes
//=========

#include "Camera.h"

#include <Engine/CZPhysics/BoxCollider.h>

eae6320::Assets::Camera::Camera(const float i_fieldOfView_y, const float i_aspectRatio, const float i_z_nearPlane, const float i_z_farPlane)
	:
	m_fieldOfView_y(i_fieldOfView_y),
	m_aspectRatio(i_aspectRatio),
	m_z_nearPlane(i_z_nearPlane),
	m_z_farPlane(i_z_farPlane)
{
	m_rigidBody = new ChrisZ::Physics::RigidBody(this);
	m_collider = new ChrisZ::Physics::BoxCollider(eae6320::Math::sVector::sVector(), eae6320::Math::sVector(0.1f, 0.1f, 0.1f), this);
	m_rigidBody->SetRotationLocked(false, false, true);
}