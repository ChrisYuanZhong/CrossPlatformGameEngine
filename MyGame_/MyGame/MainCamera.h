#pragma once

#include <Engine/Assets/Camera.h>

class MainCamera : public eae6320::Assets::Camera
{
public:
	float springArmLength = 3.0f;
	float springConstant = 2.0f;
	float springDamping = 6.0f;
	GameObject* followObject = nullptr;

	MainCamera()
	{
		m_rigidBody->SetDragCoefficient(0.0f);
		SetPosition(eae6320::Math::sVector(0.0f, 0.0f, 3.0f));
	}

	void Update(const float i_secondCountToIntegrate) override;

	void Reset();
};