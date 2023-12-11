#include "Plane.h"

Plane::Plane()
{
	m_collider = new ChrisZ::Physics::BoxCollider(eae6320::Math::sVector(), eae6320::Math::sVector(1.5f, 0.1f, 1.0f), this);

	eae6320::Graphics::Mesh::LoadFromFile(m_mesh, "data/Meshes/Plane.mesh");
	eae6320::Graphics::Effect::Load(m_effect, "data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/standard.shader");

	m_transform->SetPosition(eae6320::Math::sVector(0.0f, -1.0f, 0.0f));
	m_collider->SetRestitution(0.3f);
	m_collider->SetFrictionCoefficient(0.2f);
}