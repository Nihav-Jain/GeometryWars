#include "pch.h"
#include "Game.h"
#include "Renderer.h"
#include "OpenGLRenderDevice.h"

using namespace Library;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{
	UNREFERENCED_PARAMETER(instance);
	UNREFERENCED_PARAMETER(previousInstance);
	UNREFERENCED_PARAMETER(commandLine);
	UNREFERENCED_PARAMETER(showCommand);

	Game game;
	game.Start();

	OpenGLRenderDevice renderDevice;

	renderDevice.InitOpenGl();

	Renderer render(&renderDevice);

#pragma warning(push)
#pragma warning(disable : 4127)
	while (true) { // TODO remove always true
#pragma warning(pop) 
		render.Update();
	}

	return 0;
}