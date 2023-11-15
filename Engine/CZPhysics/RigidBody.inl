#pragma once

#include "RigidBody.h"

inline void ChrisZ::Physics::RigidBody::AddForce(const eae6320::Math::sVector i_force)
{
	// Update force
	force += i_force;
}

inline void ChrisZ::Physics::RigidBody::AddImpulse(const eae6320::Math::sVector i_impulse)
{
	// Update velocity
	velocity += i_impulse / mass;
}

// Getters and Setters
//====================

inline eae6320::Math::sVector ChrisZ::Physics::RigidBody::GetVelocity() const
{
	return velocity;
}

inline void ChrisZ::Physics::RigidBody::SetVelocity(eae6320::Math::sVector i_velocity)
{
	velocity = i_velocity;
}

inline eae6320::Math::sVector ChrisZ::Physics::RigidBody::GetAcceleration() const
{
	return acceleration;
}

inline void ChrisZ::Physics::RigidBody::SetAcceleration(eae6320::Math::sVector i_acceleration)
{
	acceleration = i_acceleration;
}

inline eae6320::Math::sVector ChrisZ::Physics::RigidBody::GetAngularVelocity_axis_local() const
{
	return angularVelocity_axis_local;
}

inline void ChrisZ::Physics::RigidBody::SetAngularVelocity_axis_local(eae6320::Math::sVector i_angularVelocity_axis_local)
{
	angularVelocity_axis_local = i_angularVelocity_axis_local;
}

inline float ChrisZ::Physics::RigidBody::GetAngularSpeed() const
{
	return angularSpeed;
}

inline void ChrisZ::Physics::RigidBody::SetAngularSpeed(float i_angularSpeed)
{
	angularSpeed = i_angularSpeed;
}

inline float ChrisZ::Physics::RigidBody::GetDragCoefficient() const
{
	return dragCoefficient;
}

inline void ChrisZ::Physics::RigidBody::SetDragCoefficient(float i_dragCoefficient)
{
	dragCoefficient = i_dragCoefficient;
}

inline float ChrisZ::Physics::RigidBody::GetMass() const
{
	return mass;
}

inline void ChrisZ::Physics::RigidBody::SetMass(float i_mass)
{
	mass = i_mass;
}

inline bool ChrisZ::Physics::RigidBody::GetGravityEnabled() const
{
	return gravityEnabled;
}

inline void ChrisZ::Physics::RigidBody::SetGravityEnabled(bool i_gravityEnabled)
{
	gravityEnabled = i_gravityEnabled;
}


inline eae6320::Math::cMatrix_transformation ChrisZ::Physics::RigidBody::PredictFutureTransform(const float i_secondCountToExtrapolate) const
{
	return eae6320::Math::cMatrix_transformation(PredictFutureOrientation(i_secondCountToExtrapolate), PredictFuturePosition(i_secondCountToExtrapolate));
}