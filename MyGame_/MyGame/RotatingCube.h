#pragma once

#include <Engine/Assets/GameObject.h>

#include <Engine/CZPhysics/BoxCollider.h>

class RotatingCube : public eae6320::Assets::GameObject
{
	public:
	RotatingCube()
	{
		m_rigidBody = new ChrisZ::Physics::RigidBody(this);
		m_collider = new ChrisZ::Physics::BoxCollider(eae6320::Math::sVector::sVector(), eae6320::Math::sVector(0.5f, 0.5f, 0.5f), this);

		eae6320::Graphics::Mesh::LoadFromFile(m_mesh, "data/Meshes/Cube.mesh");
		eae6320::Graphics::Effect::Load(m_effect, "data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/animatedcolor2.shader");

		m_rigidBody->SetAngularVelocity(eae6320::Math::sVector(0.0f, 1.0f, 0.0f));

		m_collider->SetIsTrigger(true);
	}
};