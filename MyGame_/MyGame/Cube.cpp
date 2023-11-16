#include "Cube.h"

Cube::Cube()
{
	m_transform = new eae6320::Assets::Transform();
	m_rigidBody = new ChrisZ::Physics::RigidBody(this);
	m_collider = new ChrisZ::Physics::BoxCollider(eae6320::Math::sVector::sVector(), eae6320::Math::sVector(0.5f, 0.5f, 0.5f), this);

	eae6320::Graphics::Mesh::LoadFromFile(m_mesh, "data/Meshes/Cube.mesh");
	eae6320::Graphics::Effect::Load(m_effect, "data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/animatedcolor1.shader");

	//m_rigidBody->SetAngularSpeed(1.0f);
	//m_transform->SetOrientation(eae6320::Math::cQuaternion(0.0f, 0.0f, 0.0f, 1.0f));
	m_rigidBody->SetMass(20.0f);
	m_collider->SetFrictionCoefficient(0.0f);
	m_collider->SetRestitution(0.0f);
}