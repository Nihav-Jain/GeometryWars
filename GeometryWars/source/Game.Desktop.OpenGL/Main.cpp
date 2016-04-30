#include "pch.h"
#include "Game.h"
#include "InputManager.h"
#include "ActionDebug.h"
#include "Renderer.h"
#include "OpenGLRenderDevice.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

using namespace Library;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{
	UNREFERENCED_PARAMETER(instance);
	UNREFERENCED_PARAMETER(previousInstance);
	UNREFERENCED_PARAMETER(commandLine);
	UNREFERENCED_PARAMETER(showCommand);

	OpenGLRenderDevice renderDevice;
	renderDevice.InitOpenGl();

	/// Game-specific factories that cannot live as members in the Game class ///
	XBoxControllerHandlerFactory xchf;
	ActionDebugFactory adf;
	PlayerFactory mPlayerFactory;
	EnemyFactory mEnemyFactory;
	BulletFactory mBulletFactory;

	// TODO: Remove singleton!!!!!!!!!!!!!!!!!!!! By Yuhsiang
	Renderer * renderer = Renderer::GetInstance(&renderDevice);

	Game game;
	game.SetRenderer(renderer);
	game.Start("Content/config/player_test.xml");

#pragma warning(push)
#pragma warning(disable : 4127)
	while (true) { // TODO remove always true
#pragma warning(pop) 
		game.Update();
	}

	return 0;
}