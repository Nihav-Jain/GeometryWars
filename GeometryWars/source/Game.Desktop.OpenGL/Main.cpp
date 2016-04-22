#include "pch.h"
#include "Game.h"
#include "InputManager.h"
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

	Game game;

	KeyBoardHandlerFactory khf;
	XBoxControllerHandlerFactory xchf;

	OutputDebugString(L"LOOK AT ME!");
	OutputDebugStringA("HELPOPO AT ME!");
	OutputDebugStringW(L"WORLD AT ME!");
	game.Start();

	Sprite sprite;
	sprite.SetColor(glm::vec4(1, 0, 0, 0));
	sprite.SetImagePath("Content/resource/mushroom.png");

	OpenGLRenderDevice renderDevice;
	renderDevice.InitOpenGl();

	Renderer render(&renderDevice);
	render.AddRenderable(&sprite);

	glm::vec4 pos(1, 0, 0, 0);
#pragma warning(push)
#pragma warning(disable : 4127)
	while (true) { // TODO remove always true
#pragma warning(pop) 
		render.Update();
		pos.x += 1;
		sprite.SetPosition(pos);
	}

	return 0;
}