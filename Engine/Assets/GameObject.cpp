#include "GameObject.h"

eae6320::Assets::GameObject::GameObject()
{
	m_isValid = true;
	m_transform = new Transform();
	m_rigidBody = nullptr;
	m_collider = nullptr;

	m_mesh = nullptr;
	m_effect = nullptr;
}

eae6320::Assets::GameObject::GameObject(Graphics::Mesh* i_mesh, Graphics::Effect* i_effect)
{
	m_isValid = true;
	m_transform = new Transform();
	m_rigidBody = nullptr;
	m_collider = nullptr;

	m_mesh = i_mesh;
	m_effect = i_effect;
}

eae6320::Assets::GameObject::~GameObject()
{
	if (m_mesh)
		m_mesh->DecrementReferenceCount();
	if (m_effect)
		m_effect->DecrementReferenceCount();

	if (m_transform)
		delete m_transform;
	if (m_rigidBody)
		delete m_rigidBody;
	if (m_collider)
		delete m_collider;
}