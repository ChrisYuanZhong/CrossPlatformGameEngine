#pragma once

// Includes
//=========

#include "GameObject.h"

#include <Engine/Math/Functions.h>
#include <Engine/Math/cMatrix_transformation.h>
//#include <Engine/Physics/sRigidBodyState.h>

namespace eae6320
{
	namespace Assets
	{
		class Camera : public GameObject
		{
		public:
			Camera(const float i_fieldOfView_y = 1.27f, const float i_aspectRatio = 16.0f / 9.0f, const float i_z_nearPlane = 0.1f, const float i_z_farPlane = 1000.0f);
			~Camera() = default;

			inline Math::cMatrix_transformation GetWorldToCameraTransformPrediction(const float i_secondCountToExtrapolate);
			inline Math::cMatrix_transformation GetCameraToProjectedTransform();

		protected:
			const float m_aspectRatio = 16.0f / 9.0f;
			const float m_z_nearPlane = 0.1f;
			const float m_z_farPlane = 1000.0f;
			const float m_fieldOfView_y = Math::ConvertHorizontalFieldOfViewToVerticalFieldOfView(Math::ConvertDegreesToRadians( 80.0f ), m_aspectRatio);
		};
	}
}

inline eae6320::Math::cMatrix_transformation eae6320::Assets::Camera::GetWorldToCameraTransformPrediction(const float i_secondCountToExtrapolate)
{
	return Math::cMatrix_transformation::CreateWorldToCameraTransform(m_rigidBody->PredictFutureTransform(i_secondCountToExtrapolate));
}

inline eae6320::Math::cMatrix_transformation eae6320::Assets::Camera::GetCameraToProjectedTransform()
{
	return Math::cMatrix_transformation::CreateCameraToProjectedTransform_perspective(m_fieldOfView_y, m_aspectRatio, m_z_nearPlane, m_z_farPlane);
}