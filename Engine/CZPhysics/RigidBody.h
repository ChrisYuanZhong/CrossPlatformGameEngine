#pragma once

#include <Engine/Math/cQuaternion.h>
#include <Engine/Math/sVector.h>

// Forward Declarations
//=====================

namespace eae6320
{
	namespace Math
	{
		class cMatrix_transformation;
	}

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
			void AddForce(const eae6320::Math::sVector i_force);
			void AddForceAtLocation(const eae6320::Math::sVector i_force, const eae6320::Math::sVector i_pointOfImpact);
			void AddImpulse(const eae6320::Math::sVector i_impulse);
			void AddImpulseAtLocation(const eae6320::Math::sVector i_impulse, const eae6320::Math::sVector i_pointOfImpact);

			// Getters and Setters
			//====================

			eae6320::Assets::GameObject* GetGameObject() const { return gameObject; }
			eae6320::Math::sVector GetVelocity() const { return velocity; }
			void SetVelocity(eae6320::Math::sVector i_velocity) { velocity = i_velocity; }
			eae6320::Math::sVector GetAcceleration() const { return acceleration; }
			void SetAcceleration(eae6320::Math::sVector i_acceleration) { acceleration = i_acceleration; }
			eae6320::Math::sVector GetAngularVelocity_axis_local() const { return angularVelocity_axis_local; }
			void SetAngularVelocity_axis_local(eae6320::Math::sVector i_angularVelocity_axis_local) { angularVelocity_axis_local = i_angularVelocity_axis_local; }
			float GetAngularSpeed() const { return angularSpeed; }
			void SetAngularSpeed(float i_angularSpeed) { angularSpeed = i_angularSpeed; }
			float GetDragCoefficient() const { return dragCoefficient; }
			void SetDragCoefficient(float i_dragCoefficient) { dragCoefficient = i_dragCoefficient; }
			float GetMass() const { return mass; }
			void SetMass(float i_mass) { mass = i_mass; }
			bool GetGravityEnabled() const { return gravityEnabled; }
			void SetGravityEnabled(bool i_gravityEnabled) { gravityEnabled = i_gravityEnabled; }

			// Interface
			//==========

			void Update(const float i_secondCountToIntegrate);
			eae6320::Math::sVector PredictFuturePosition(const float i_secondCountToExtrapolate) const;
			eae6320::Math::cQuaternion PredictFutureOrientation(const float i_secondCountToExtrapolate) const;
			eae6320::Math::cMatrix_transformation PredictFutureTransform(const float i_secondCountToExtrapolate) const;

		private:
			eae6320::Assets::GameObject* gameObject = nullptr;

			eae6320::Math::sVector velocity;	// Distance per second
			eae6320::Math::sVector acceleration;	// Distance per second^2
			eae6320::Math::sVector angularVelocity_axis_local;	// In local space (not world space)
			float angularSpeed;	// Radians per second (positive values rotate right-handed, negative rotate left-handed)
			float dragCoefficient;	// Drag is the force that resists movement through a fluid
			float mass;
			bool gravityEnabled;
			float g;	// m/s^2
		};
	}
}