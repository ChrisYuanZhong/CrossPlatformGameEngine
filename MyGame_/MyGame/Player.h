// Includes
//=========

#include "Plane.h"
#include "Cube.h"

#include <Engine/CZPhysics/SphereCollider.h>
#include <Engine/CZPhysics/BoxCollider.h>

#include <Engine/Assets/GameObject.h>
#include <Engine/UserInput/UserInput.h>

class Player : public eae6320::Assets::GameObject
{
public:
	float speed = 5.0f;
	eae6320::Graphics::Effect* originalEffect;
	eae6320::Graphics::Effect* alternativeEffect;

	Player()
	{
		m_transform = new eae6320::Assets::Transform();
		m_rigidBody = new ChrisZ::Physics::RigidBody(this);
		//m_collider = new ChrisZ::Physics::SphereCollider(eae6320::Math::sVector::sVector(), 0.5f, this);
		m_collider = new ChrisZ::Physics::BoxCollider(eae6320::Math::sVector::sVector(), eae6320::Math::sVector(0.2f, 0.5f, 0.2f), this);

		eae6320::Graphics::Mesh::LoadFromFile(m_mesh, "data/Meshes/ThinLongCube.mesh");
		eae6320::Graphics::Effect::Load(m_effect, "data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/animatedcolor1.shader");

		originalEffect = m_effect;
		eae6320::Graphics::Effect::Load(alternativeEffect, "data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/standard.shader");

		m_rigidBody->SetGravityEnabled(true);
		m_rigidBody->SetRotationLocked(true, false, true);

		// Set the orientation of the cube to be 45 degrees around the y-axis
		//m_transform->SetOrientation(eae6320::Math::cQuaternion(0.785398163f, eae6320::Math::sVector(0.0f, 1.0f, 0.0f)));

	}

	~Player()
	{
		if (m_effect != alternativeEffect)
			alternativeEffect->DecrementReferenceCount();
		else
			originalEffect->DecrementReferenceCount();
	}

private:
	bool isJumping = false;

	void Update(const float i_secondCountToIntegrate) override
	{
		// Movement

		// Move left
		if (eae6320::UserInput::IsKeyPressed('A'))
		{
			m_rigidBody->AddForce(eae6320::Math::sVector(-speed, 0.0f, 0.0f));
		}
		// Move right
		if (eae6320::UserInput::IsKeyPressed('D'))
		{
			m_rigidBody->AddForce(eae6320::Math::sVector(speed, 0.0f, 0.0f));
		}
		// Move forward
		if (eae6320::UserInput::IsKeyPressed('W'))
		{
			m_rigidBody->AddForce(eae6320::Math::sVector(0.0f, 0.0f, -speed));
		}
		// Move backward
		if (eae6320::UserInput::IsKeyPressed('S'))
		{
			m_rigidBody->AddForce(eae6320::Math::sVector(0.0f, 0.0f, speed));
		}

		// Jump
		if (eae6320::UserInput::IsKeyPressed(VK_SPACE))
		{
			Jump();
		}
	}

	void OnCollisionEnter(ChrisZ::Physics::Collider* other) override
	{
		if (dynamic_cast<Plane*>(other->GetGameObject()))
		{
			isJumping = false;
		}
		if (dynamic_cast<Cube*>(other->GetGameObject()))
		{
			m_effect = alternativeEffect;
		}
	}

	void OnCollisionExit(ChrisZ::Physics::Collider* other) override
	{
		if (dynamic_cast<Cube*>(other->GetGameObject()))
		{
			m_effect = originalEffect;
		}
	}

	void Jump()
	{
		if (!isJumping)
		{
			m_rigidBody->AddImpulse(eae6320::Math::sVector(0.0f, 7.0f, 0.0f));
			isJumping = true;
		}
	}
};