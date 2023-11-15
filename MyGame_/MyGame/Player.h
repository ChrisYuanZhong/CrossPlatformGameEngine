// Includes
//=========

#include "Plane.h"

#include <Engine/Assets/GameObject.h>
#include <Engine/UserInput/UserInput.h>

class Player : public eae6320::Assets::GameObject
{
private:
	bool isJumping = false;

public:
	float speed = 0.02f;

	Player()
	{
		m_transform = new eae6320::Assets::Transform();
		m_rigidBody = new ChrisZ::Physics::RigidBody(this);
		m_collider = new ChrisZ::Physics::BoxCollider(eae6320::Math::sVector::sVector(), eae6320::Math::sVector(0.2f, 0.5f, 0.2f), this);

		eae6320::Graphics::Mesh::LoadFromFile(m_mesh, "data/Meshes/ThinLongCube.mesh");
		eae6320::Graphics::Effect::Load(m_effect, "data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/animatedcolor1.shader");

		m_rigidBody->SetGravityEnabled(true);
	}

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
	}

	void Jump()
	{
		if (!isJumping)
		{
			m_rigidBody->AddForce(eae6320::Math::sVector(0.0f, 5.0f, 0.0f));
			isJumping = true;
		}
	}
};