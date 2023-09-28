#pragma once

// Includes
//=========

#include <Engine/Graphics/Mesh.h>
#include <Engine/Graphics/Effect.h>

namespace eae6320
{
	namespace Assets
	{
		class GameObject
		{
		public:
			GameObject(Graphics::Mesh* i_mesh, Graphics::Effect* i_effect);
			~GameObject();
			


		private:
			Graphics::Mesh* m_mesh;
			Graphics::Effect* m_effect;
		};
	}
}