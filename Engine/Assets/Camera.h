#pragma once

// Includes
//=========

#include <Engine/Math/Functions.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Physics/sRigidBodyState.h>

namespace eae6320
{
	namespace Assets
	{
		class Camera
		{
		public:
			Camera() { m_rigidBodyState.position.z = 2.0f; }
			~Camera() = default;

			Math::cMatrix_transformation GetWorldToCameraTransformPrediction(const float i_secondCountToExtrapolate);
			Math::cMatrix_transformation GetCameraToProjectedTransform();

			// Rigid Body State
			inline Math::sVector GetPosition() const { return m_rigidBodyState.position; }
			inline void SetPosition(Math::sVector i_position) { m_rigidBodyState.position = i_position; }
			inline Math::cQuaternion GetOrientation() const { return m_rigidBodyState.orientation; }
			inline void SetOrientation(Math::cQuaternion i_orientation) { m_rigidBodyState.orientation = i_orientation; }
			inline Math::sVector GetVelocity() const { return m_rigidBodyState.velocity; }
			inline void SetVelocity(Math::sVector i_velocity) { m_rigidBodyState.velocity = i_velocity; }
			inline Math::sVector GetAcceleration() const { return m_rigidBodyState.acceleration; }
			inline void SetAcceleration(Math::sVector i_acceleration) { m_rigidBodyState.acceleration = i_acceleration; }
			
			inline void Update(const float i_secondCountToIntegrate) { m_rigidBodyState.Update(i_secondCountToIntegrate); }

		private:
			const float m_aspectRatio = 1.0f;
			const float m_z_nearPlane = 0.1f;
			const float m_z_farPlane = 1000.0f;
			const float m_fieldOfView_y = Math::ConvertHorizontalFieldOfViewToVerticalFieldOfView(Math::ConvertDegreesToRadians( 80.0f ), m_aspectRatio);

			Physics::sRigidBodyState m_rigidBodyState;
		};
	}
}