// Includes
//=========

#include "Camera.h"

eae6320::Math::cMatrix_transformation eae6320::Assets::Camera::GetWorldToCameraTransformPrediction(const float i_secondCountToExtrapolate)
{
	return Math::cMatrix_transformation::CreateWorldToCameraTransform(m_rigidBodyState.PredictFutureTransform(i_secondCountToExtrapolate));
}

eae6320::Math::cMatrix_transformation eae6320::Assets::Camera::GetCameraToProjectedTransform()
{
	return Math::cMatrix_transformation::CreateCameraToProjectedTransform_perspective(m_fieldOfView_y, m_aspectRatio, m_z_nearPlane, m_z_farPlane);
}
