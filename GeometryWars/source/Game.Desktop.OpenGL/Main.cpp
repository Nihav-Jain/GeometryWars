#include "pch.h"
#include "Game.h"
#include "InputManager.h"
#include "ActionDebug.h"
#include "Renderer.h"
#include "OpenGLRenderDevice.h"

using namespace Library;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{
	UNREFERENCED_PARAMETER(instance);
	UNREFERENCED_PARAMETER(previousInstance);
	UNREFERENCED_PARAMETER(commandLine);
	UNREFERENCED_PARAMETER(showCommand);

	OpenGLRenderDevice renderDevice;
	renderDevice.InitOpenGl();

	Game game;
	game.SetRendererDevice(&renderDevice);
	game.Start("Content/config/polygon.xml");

#pragma warning(push)
#pragma warning(disable : 4127)
	while (true) { // TODO remove always true
#pragma warning(pop) 
		game.Update();
	}

	return 0;
}