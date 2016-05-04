#include "pch.h"
#include "Game.h"
#include "InputManager.h"
#include "ActionDebug.h"
#include "Renderer.h"
#include "OpenGLRenderDevice.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemySquare.h"
#include "Collectible.h"
#include "Bullet.h"
#include "BloomPostProcessing.h"

using namespace Library;
using namespace OpenGLImplmentation;



int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{
	UNREFERENCED_PARAMETER(instance);
	UNREFERENCED_PARAMETER(previousInstance);
	UNREFERENCED_PARAMETER(commandLine);
	UNREFERENCED_PARAMETER(showCommand);

	OpenGLRenderDevice renderDevice;

	/// Game-specific factories that cannot live as members in the Game class ///
	XBoxControllerHandlerFactory xchf;
	ActionDebugFactory adf;
	PlayerFactory mPlayerFactory;
	EnemyFactory mEnemyFactory;
	EnemySquareFactory mEnemySquareFactory;
	BulletFactory mBulletFactory;
	CollectibleFactory mCollectibleFactory;

	BloomPostProcessing bloom;

	// TODO: Remove singleton!!!!!!!!!!!!!!!!!!!! By Yuhsiang
	Renderer * renderer = Renderer::GetInstance(&renderDevice);

	renderer->AddPostPostProcessing(&bloom);

	/// Game ///
	Game game;
	game.SetRenderer(renderer);
	game.Start("Content/config/geometrywars_test.xml");
	renderDevice.InitOpenGl("Geometry Wars", game.GetWorld().GetWidth(), game.GetWorld().GetHeight());

#pragma warning(push)
#pragma warning(disable : 4127)
	while (renderDevice.IsValid())
	{
#pragma warning(pop) 
		game.Update();
	}

	return 0;
}


