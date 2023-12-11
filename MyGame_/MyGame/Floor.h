#pragma once

#include "PlatformerGameObject.h"

#include <Engine/CZPhysics/BoxCollider.h>

class Floor : public PlatformerGameObject
{
public:
	Floor()
	{
		m_collider = new ChrisZ::Physics::BoxCollider(eae6320::Math::sVector::sVector(), eae6320::Math::sVector(5.0f, 0.25f, 0.5f), this);

		eae6320::Graphics::Mesh::LoadFromFile(m_mesh, "data/Meshes/Floor.mesh");
		eae6320::Graphics::Effect::Load(m_effect, "data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/standard.shader");

		m_collider->SetFrictionCoefficient(0.2f);
	}
};