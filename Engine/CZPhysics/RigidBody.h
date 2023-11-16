#pragma once

#include <Engine/Math/cQuaternion.h>
#include <Engine/Math/sVector.h>
#include <Engine/Math/cMatrix_transformation.h>

// Forward Declarations
//=====================

namespace eae6320
{
	namespace Assets
	{
		class GameObject;
	}
}

namespace ChrisZ
{
	namespace Physics
	{
		class RigidBody
		{
		public:
			// Initialization
			//===============

			RigidBody(eae6320::Assets::GameObject* i_gameObject);

			// Interface
			//==========
			inline void AddForce(const eae6320::Math::sVector i_force);
			//void AddForceAtLocation(const eae6320::Math::sVector i_force, const eae6320::Math::sVector i_pointOfImpact);
			inline void AddImpulse(const eae6320::Math::sVector i_impulse);
			//void AddImpulseAtLocation(const eae6320::Math::sVector i_impulse, const eae6320::Math::sVector i_pointOfImpact);

			// Getters and Setters
			//====================

			eae6320::Assets::GameObject* GetGameObject() const;
			inline eae6320::Math::sVector GetVelocity() const;
			inline void SetVelocity(eae6320::Math::sVector i_velocity);
			inline eae6320::Math::sVector GetAcceleration() const;
			inline void SetAcceleration(eae6320::Math::sVector i_acceleration);
			inline eae6320::Math::sVector GetAngularVelocity_axis_local() const;
			inline void SetAngularVelocity_axis_local(eae6320::Math::sVector i_angularVelocity_axis_local);
			inline float GetAngularSpeed() const;
			inline void SetAngularSpeed(float i_angularSpeed);
			inline float GetDragCoefficient() const;
			inline void SetDragCoefficient(float i_dragCoefficient);
			inline float GetMass() const;
			inline void SetMass(float i_mass);
			inline bool GetGravityEnabled() const;
			inline void SetGravityEnabled(bool i_gravityEnabled);

			// Interface
			//==========

			void Update(const float i_secondCountToIntegrate);
			eae6320::Math::sVector PredictFuturePosition(const float i_secondCountToExtrapolate) const;
			eae6320::Math::cQuaternion PredictFutureOrientation(const float i_secondCountToExtrapolate) const;
			inline eae6320::Math::cMatrix_transformation PredictFutureTransform(const float i_secondCountToExtrapolate) const;

		private:
			eae6320::Assets::GameObject* gameObject = nullptr;

			eae6320::Math::sVector velocity;	// Distance per second
			eae6320::Math::sVector acceleration;	// Distance per second^2
			eae6320::Math::sVector angularVelocity_axis_local;	// In local space (not world space)
			float angularSpeed;	// Radians per second (positive values rotate right-handed, negative rotate left-handed)
			float dragCoefficient;	// Drag is the force that resists movement
			float mass;
			eae6320::Math::sVector force;
			bool gravityEnabled;
			float g;	// m/s^2
		};
	}
}

#include "RigidBody.inl"