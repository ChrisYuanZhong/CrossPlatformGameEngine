#include "Cube.h"

Cube::Cube()
{
	m_transform = new eae6320::Assets::Transform();
	m_rigidBody = new ChrisZ::Physics::RigidBody(this);
	m_collider = new ChrisZ::Physics::BoxCollider(eae6320::Math::sVector::sVector(), eae6320::Math::sVector(0.5f, 0.5f, 0.5f), this);

	eae6320::Graphics::Mesh::LoadFromFile(m_mesh, "data/Meshes/Cube.mesh");
	eae6320::Graphics::Effect::Load(m_effect, "data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/animatedcolor2.shader");

	// Set the orientation of the cube to be 45 degrees around the y-axis
	//m_transform->SetOrientation(eae6320::Math::cQuaternion(0.785398163f, eae6320::Math::sVector(0.0f, 1.0f, 0.0f)));

	//	Set the orientation of the cube to be 180 degrees around the x-axis
	//m_transform->SetOrientation(eae6320::Math::cQuaternion(3.14159265f, eae6320::Math::sVector(1.0f, 0.0f, 0.0f)));

	m_rigidBody->SetAngularVelocity(eae6320::Math::sVector(0.0f, 1.0f, 0.0f));
	m_rigidBody->SetRotationLocked(true, false, true);
	m_rigidBody->SetMass(20.0f);
	m_collider->SetFrictionCoefficient(0.0f);
	m_collider->SetRestitution(0.0f);
}