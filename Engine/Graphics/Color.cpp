#include "Color.h"

eae6320::Graphics::RGBA eae6320::Graphics::convertHexToRGBA(unsigned int hexColor)
{
    RGBA rgbaColor{};

    // Check if the alpha component is present
    if (hexColor > 0xFFFFFF)
	    {
            rgbaColor.R = static_cast<float>(((hexColor >> 24) & 0xFF) / 255.0);  // Extract the red component
            rgbaColor.G = static_cast<float>(((hexColor >> 16) & 0xFF) / 255.0);   // Extract the green component
            rgbaColor.B = static_cast<float>(((hexColor >> 8) & 0xFF) / 255.0);          // Extract the blue component
		    rgbaColor.A = static_cast<float>(((hexColor & 0xFF) & 0xFF) / 255.0);  // Extract the alpha component
	    }
    else
	    {
			rgbaColor.R = static_cast<float>(((hexColor >> 16) & 0xFF) / 255.0);   // Extract the green component
			rgbaColor.G = static_cast<float>(((hexColor >> 8) & 0xFF) / 255.0);          // Extract the blue component
		    rgbaColor.B = static_cast<float>(((hexColor & 0xFF) & 0xFF) / 255.0);  // Extract the alpha component
		    rgbaColor.A = 1.0f;
	    }

    return rgbaColor;
}
