#pragma once
/*
	This file declares the external interface for the physics system
*/

// Includes
//=========

#include "RigidBody.h"
#include "SphereCollider.h"
#include "BoxCollider.h"

namespace ChrisZ
{
	namespace Physics
	{
		void AddRigidBody(RigidBody* i_rigidBody);
		void AddCollider(Collider* i_collider);

		void Update(const float i_secondCountToIntegrate);
	}
}