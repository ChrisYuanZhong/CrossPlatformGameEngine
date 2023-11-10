#include "GameObject.h"

eae6320::Assets::GameObject::GameObject()
{
	m_mesh = nullptr;
	m_effect = nullptr;

	ChrisZ::Physics::AddCollider(&m_sphereCollider);
	ChrisZ::Physics::AddRigidBody(&m_rigidBody);
}

eae6320::Assets::GameObject::GameObject(Graphics::Mesh* i_mesh, Graphics::Effect* i_effect)
{
	m_mesh = i_mesh;
	m_effect = i_effect;

	ChrisZ::Physics::AddCollider(&m_sphereCollider);
	ChrisZ::Physics::AddRigidBody(&m_rigidBody);
}

eae6320::Assets::GameObject::~GameObject()
{
	m_mesh->DecrementReferenceCount();
	m_effect->DecrementReferenceCount();
}
