#pragma once

#include "Graphics.h"

namespace eae6320
{
	namespace Graphics {
		struct RGBA
		{
			float R;
			float G;
			float B;
			float A;
		};

		RGBA convertHexToRGBA(unsigned int hexColor);
	}
}