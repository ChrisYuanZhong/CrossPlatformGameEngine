#pragma once

// Includes
//=========

#include <Engine/Math/sVector.h>
#include <Engine/Math/cQuaternion.h>

namespace eae6320
{
	namespace Assets
	{
		class Transform
		{
		public:
			// Interface
			//==========

			// Default constructor
			Transform() :
				position(Math::sVector(0.0f, 0.0f, 0.0f))
			{

			}

			Transform(float i_x, float i_y, float i_z) :
				position(Math::sVector(i_x, i_y, i_z))
			{

			}

			// Data
			//=====

			// Position
			//---------

			Math::sVector GetPosition() const { return position; }
			void SetPosition(const Math::sVector& i_position) { position = i_position; }

			// Orientation
			//------------

			Math::cQuaternion GetOrientation() const { return orientation; }
			void SetOrientation(const Math::cQuaternion& i_orientation) { orientation = i_orientation; }

		private:
			Math::sVector position;	// In arbitrary units determined by the applicaton's convention
			Math::cQuaternion orientation;	// In radians
		};
	}
}