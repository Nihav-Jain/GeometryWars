#include "pch.h"
#include "Game.h"
#include "Renderer.h"
#include "OpenGLRenderDevice.h"
#include "Sprite.h" // TODO just for testing

using namespace Library;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{
	UNREFERENCED_PARAMETER(instance);
	UNREFERENCED_PARAMETER(previousInstance);
	UNREFERENCED_PARAMETER(commandLine);
	UNREFERENCED_PARAMETER(showCommand);

	//Game game;
	//game.Start();

	Sprite sprite;
	sprite.SetColor(glm::vec4(1, 0, 0, 0));
	sprite.SetImagePath("Content/resource/mushroom.png");
	sprite.SetPosition(glm::vec4(0, 0, 0, 0));

	OpenGLRenderDevice renderDevice;
	renderDevice.InitOpenGl();

	Renderer render(&renderDevice);
	render.AddRenderable(&sprite);

#pragma warning(push)
#pragma warning(disable : 4127)
	while (true) { // TODO remove always true
#pragma warning(pop) 
		render.Update();
	}

	return 0;
}