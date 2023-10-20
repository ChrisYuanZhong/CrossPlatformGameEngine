// Includes
//=========

#include "cMeshBuilder.h"

#include <Tools/AssetBuildLibrary/Functions.h>
#include <Engine/Platform/Platform.h>

// Inherited Implementation
//=========================

// Build
//------

eae6320::cResult eae6320::Assets::cMeshBuilder::Build(const std::vector<std::string>& i_arguments)
{
	//Copy the source file to the target file
	{
		std::string errorMessage;
		if (!Platform::CopyFile(m_path_source, m_path_target, false, true, &errorMessage))
		{
			OutputErrorMessageWithFileInfo(m_path_source, errorMessage.c_str());
			return Results::Failure;
		}
		return Results::Success;
	}
}