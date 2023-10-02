#include "GameObject.h"

eae6320::Assets::GameObject::GameObject(Graphics::Mesh* i_mesh, Graphics::Effect* i_effect)
{
	m_mesh = i_mesh;
	m_effect = i_effect;
}

eae6320::Assets::GameObject::~GameObject()
{
	m_mesh->DecrementReferenceCount();
	m_effect->DecrementReferenceCount();
}
