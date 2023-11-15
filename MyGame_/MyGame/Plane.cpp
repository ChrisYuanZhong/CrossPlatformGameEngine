#include "Plane.h"

Plane::Plane()
{
	m_transform = new eae6320::Assets::Transform(0.0f, -1.0f, 0.0f);
	//m_rigidBody = new ChrisZ::Physics::RigidBody(this);
	m_collider = new ChrisZ::Physics::BoxCollider(GetPosition(), eae6320::Math::sVector(1.5f, 0.01f, 1.0f), this);

	eae6320::Graphics::Mesh::LoadFromFile(m_mesh, "data/Meshes/Plane.mesh");
	eae6320::Graphics::Effect::Load(m_effect, "data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/standard.shader");

	m_transform->SetPosition(eae6320::Math::sVector(0.0f, -1.0f, 0.0f));
	m_collider->SetRestitution(0.5f);
	m_collider->SetFrictionCoefficient(0.5f);
	//m_rigidBody->SetMass(5.0f);
}