#pragma once

#include "PlatformerGameObject.h"

#include <Engine/CZPhysics/BoxCollider.h>

class Checkpoint : public PlatformerGameObject
{
public:
	Checkpoint()
	{
		m_collider = new ChrisZ::Physics::BoxCollider(eae6320::Math::sVector::sVector(), eae6320::Math::sVector(0.2f, 1.0f, 0.5f), this);

		m_collider->SetIsTrigger(true);
	}
};