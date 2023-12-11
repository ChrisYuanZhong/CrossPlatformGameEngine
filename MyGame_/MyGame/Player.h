// Includes
//=========

#include "Plane.h"
#include "Cube.h"
#include "RotatingCube.h"
#include "Spike.h"
#include "MainCamera.h"
#include "Checkpoint.h"

#include <Engine/CZPhysics/BoxCollider.h>

#include <Engine/Audio/cAudioSystem.h>

#include "PlatformerGameObject.h"
#include <Engine/UserInput/UserInput.h>

class Player : public PlatformerGameObject
{
public:
	float speed = 0.6f;
	float jumpMomentum = 5.0f;
	float doubleJumpMomentum = 4.0f;
	bool canDoubleJump = true;
	float maxHoldJumpTime = 0.3f;
	MainCamera* mainCamera = nullptr;
	eae6320::Graphics::Effect* originalEffect;
	eae6320::Graphics::Effect* alternativeEffect;
	eae6320::AudioSystem::cAudio jumpSound1;
	eae6320::AudioSystem::cAudio jumpSound2;
	eae6320::AudioSystem::cAudio deathSound;

	Player()
	{
		m_rigidBody = new ChrisZ::Physics::RigidBody(this);
		m_collider = new ChrisZ::Physics::BoxCollider(eae6320::Math::sVector::sVector(), eae6320::Math::sVector(0.15f, 0.25f, 0.15f), this);

		eae6320::Graphics::Mesh::LoadFromFile(m_mesh, "data/Meshes/Player.mesh");
		eae6320::Graphics::Effect::Load(m_effect, "data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/animatedcolor1.shader");

		originalEffect = m_effect;
		eae6320::Graphics::Effect::Load(alternativeEffect, "data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/standard.shader");

		m_rigidBody->SetMass(0.1f);
		m_rigidBody->SetDragCoefficient(2.0f);
		m_rigidBody->SetGravityEnabled(true);
		m_rigidBody->SetRotationLocked(true, true, true);

		m_collider->SetRestitution(0.0f);

		jumpSound1.AudioConstructor("data/Audio/Jump1.wav", "jump1", 1000, false);
		jumpSound2.AudioConstructor("data/Audio/Jump2.wav", "jump2", 1000, false);
		deathSound.AudioConstructor("data/Audio/Death.wav", "death", 1000, false);

		//jumpSound1.SubmitAudioToBePlayed();
		//jumpSound2.SubmitAudioToBePlayed();
	}

	~Player()
	{
		if (m_effect != alternativeEffect)
			alternativeEffect->DecrementReferenceCount();
		else
			originalEffect->DecrementReferenceCount();
	}

	void Death()
	{
		// Death
		//this->SetIsValid(false);
		SetPosition(lastCheckpointPosition);
		m_rigidBody->SetVelocity(eae6320::Math::sVector(0.0f, 0.0f, 0.0f));

		if (mainCamera)
			mainCamera->Reset();

		// Play death sound
		//deathSound.SubmitAudioToBePlayed();
		deathSound.PlayIndependent();
	}

	eae6320::Math::sVector GetLastCheckpointPosition()
	{
		return lastCheckpointPosition;
	}

private:
	bool isJumping = false;
	bool isHoldingJump = false;
	bool canHoldJump = false;
	float holdJumpTime = 0.0f;
	eae6320::Math::sVector lastCheckpointPosition;

	bool hasRReleased = true;

	//bool hasJumped1 = false;
	//bool hasJumped2 = false;

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
		//// Move forward
		//if (eae6320::UserInput::IsKeyPressed('W'))
		//{
		//	m_rigidBody->AddForce(eae6320::Math::sVector(0.0f, 0.0f, -speed));
		//}
		//// Move backward
		//if (eae6320::UserInput::IsKeyPressed('S'))
		//{
		//	m_rigidBody->AddForce(eae6320::Math::sVector(0.0f, 0.0f, speed));
		//}

		// Reset
		if (eae6320::UserInput::IsKeyPressed('R') && hasRReleased)
		{
			Death();
			hasRReleased = false;
		}
		else if (!eae6320::UserInput::IsKeyPressed('R'))
		{
			hasRReleased = true;
		}

		// Jump
		if (eae6320::UserInput::IsKeyPressed(VK_SPACE) && !isHoldingJump)
		{
			Jump();
			isHoldingJump = true;
		}
		else if (!eae6320::UserInput::IsKeyPressed(VK_SPACE))
		{
			isHoldingJump = false;
			canHoldJump = false;
			holdJumpTime = 0.0f;
		}
		// Make the jump higher if the player is holding the jump button
		else if (isHoldingJump && canHoldJump)
		{
			if (holdJumpTime < maxHoldJumpTime)
				m_rigidBody->AddForce(eae6320::Math::sVector(0.0f, 1.2f, 0.0f));

			holdJumpTime += i_secondCountToIntegrate;
		}

		//jumpSound1.SubmitAudioToBePlayed();
		//jumpSound2.SubmitAudioToBePlayed();
	}

	void OnCollisionEnter(ChrisZ::Physics::Collider* other) override
	{
		if (PlatformerGameObject* ground = dynamic_cast<PlatformerGameObject*>(other->GetGameObject()))
		{
			if (ground->GetIsStandable() && this->GetPosition().y > ground->GetPosition().y)
			{
				isJumping = false;
				canDoubleJump = true;
			}
		}
		if (dynamic_cast<Checkpoint*>(other->GetGameObject()))
		{
			lastCheckpointPosition = other->GetCenter();
		}
		if (dynamic_cast<Spike*>(other->GetGameObject()))
		{
			Death();
		}
	}

	void OnCollisionExit(ChrisZ::Physics::Collider* other) override
	{
		if (dynamic_cast<RotatingCube*>(other->GetGameObject()))
		{
			m_effect = originalEffect;
		}
	}

	void Jump()
	{
		if (!isJumping)
		{
			m_rigidBody->SetVelocity(eae6320::Math::sVector(m_rigidBody->GetVelocity().x, jumpMomentum, m_rigidBody->GetVelocity().z));
			isJumping = true;
			canHoldJump = true;

			// Play jump sound 1
			//if (hasJumped1)
				//jumpSound1.CloseAudio();
			//jumpSound1.Play();
			//hasJumped1 = true;
		}
		else if (canDoubleJump)
		{
			// Double jump
			m_rigidBody->SetVelocity(eae6320::Math::sVector(m_rigidBody->GetVelocity().x, doubleJumpMomentum, m_rigidBody->GetVelocity().z));
			canDoubleJump = false;
			canHoldJump = true;

			// Play jump sound 2
			//if (hasJumped2)
				//jumpSound2.CloseAudio();
			//jumpSound2.Play();
			//hasJumped2 = true;
		}
	}
};