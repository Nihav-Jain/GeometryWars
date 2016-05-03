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


	/// Start Screen ///
	Game startMenu;
	startMenu.SetRenderer(renderer);
	startMenu.Start("Content/config/start_menu.xml");
	renderDevice.InitOpenGl("Geometry Wars", startMenu.GetWorld().GetWidth(), startMenu.GetWorld().GetHeight());

	bool & wait = startMenu.GetWorld().Find("wait")->Get<bool>();
	SpriteRenderer* title = startMenu.GetWorld().FindSector("ImageSector")->FindEntity("TitleScreen")->FindAction("titleSprite")->AssertiveAs<SpriteRenderer>();
	renderer->RemoveRenderable(title);
	renderer->RemoveRenderable(title);
	renderer->AddRenderable(title, 101);
	while (wait)
	{
		startMenu.Update();
	}
	renderer->RemoveRenderable(title, 101);
	startMenu.Update();



	/// Game ///
	Game game;
	game.SetRenderer(renderer);
	game.Start("Content/config/geometrywars_test.xml");

	const std::int32_t & playerLivesLeft = game.GetWorld().FindSector("InGameSector")->FindEntity("Player1")->AssertiveAs<Player>()->Lives();
	while (playerLivesLeft > 0)
	{

		game.Update();
	}


	/// Gameover Screen ///
	Game gameoverMenu;
	gameoverMenu.SetRenderer(renderer);
	gameoverMenu.Start("Content/config/gameover_menu.xml");

#pragma warning(push)
#pragma warning(disable : 4127)
	while (true)
	{
		gameoverMenu.Update();
#pragma warning(pop) 
	}

	return 0;
}


