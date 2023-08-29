/*
	WinMain() is the entry point of a Windows program
	(instead of the familiar main() in a console program)
*/

// Includes
//=========

#include "cMyGame.h"

#include <Engine/Logging/Logging.h>

// Entry Point
//============

int WINAPI WinMain( HINSTANCE i_thisInstanceOfTheApplication, HINSTANCE, char* i_commandLineArguments, int i_initialWindowDisplayState )
{
	eae6320::Logging::OutputMessage( "Starting my game" );
	return eae6320::Application::Run<eae6320::cMyGame>( i_thisInstanceOfTheApplication, i_commandLineArguments, i_initialWindowDisplayState );
}
