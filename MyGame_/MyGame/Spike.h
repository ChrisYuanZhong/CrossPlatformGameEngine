#pragma once

#include "PlatformerGameObject.h"

#include <Engine/CZPhysics/BoxCollider.h>

class Spike : public PlatformerGameObject
{
public:
	Spike()
	{
		m_collider = new ChrisZ::Physics::BoxCollider(eae6320::Math::sVector::sVector(), eae6320::Math::sVector(0.3535f, 0.3535f, 0.3535f), this);
	
		eae6320::Graphics::Mesh::LoadFromFile(m_mesh, "data/Meshes/Spike.mesh");
		eae6320::Graphics::Effect::Load(m_effect, "data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/animatedcolor2.shader");

		m_transform->SetOrientation(eae6320::Math::cQuaternion(0.785398163f, eae6320::Math::sVector(0.0f, 0.0f, 1.0f)));

		m_collider->SetIsTrigger(true);

		SetIsStandable(false);
	}
};