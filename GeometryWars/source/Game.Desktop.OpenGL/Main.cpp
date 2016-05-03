#include "pch.h"
#include "Game.h"
#include "InputManager.h"
#include "ActionDebug.h"
#include "Renderer.h"
#include "OpenGLRenderDevice.h"
#include "Player.h"
#include "Enemy.h"
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
	BulletFactory mBulletFactory;

	BloomPostProcessing bloom;

	// TODO: Remove singleton!!!!!!!!!!!!!!!!!!!! By Yuhsiang
	Renderer * renderer = Renderer::GetInstance(&renderDevice);

	renderer->AddPostPostProcessing(&bloom);

	Game game;
	game.SetRenderer(renderer);

	game.Start("Content/config/geometrywars_test.xml");
	//game.Start("Content/config/player_test.xml");
	//game.Start("Content/config/polygon.xml"); // TODO use the final world here
	//game.Start("Content/config/input_v2.xml");

	renderDevice.InitOpenGl("Geomatry War", game.GetWorld().GetWidth(), game.GetWorld().GetHeight());

#pragma warning(push)
#pragma warning(disable : 4127)
	while (true) { // TODO remove always true
#pragma warning(pop) 
		game.Update();
	}

	return 0;
}


