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

	//<<<<<<< HEAD
	Game game;

	KeyBoardHandlerFactory khf;
	XBoxControllerHandlerFactory xchf;
	ActionDebugFactory adf;

	game.ParseMaster().ParseFromFile("../../../../../Content/config/input_v2.xml");
	game.Start();

	// For Testing Purposes Only...
	Entity& Character = *(game.GetWorld().Sectors().Get<Scope>().AssertiveAs<Sector>()->Entities().Get<Scope>().AssertiveAs<Entity>());
	//const XBoxControllerState& PlayerController = game.GetWorld().Actions().Get<Scope>().AssertiveAs<ActionList>()->Actions().Get<Scope>(1).AssertiveAs<XBoxControllerHandler>()->GetPlayerState(0);

	Sprite sprite;
	sprite.SetColor(glm::vec4(1, 0, 0, 0));
	sprite.SetImagePath("Content/resource/mushroom.png");

	//=======
	//>>>>>>> master
	OpenGLRenderDevice renderDevice;
	
	// TODO: Remove singleton!!!!!!!!!!!!!!!!!!!! By Yuhsiang
	Renderer * renderer = Renderer::GetInstance(&renderDevice);

	Game game;
	game.SetRenderer(renderer);
	game.Start("Content/config/polygon.xml"); // TODO use the final world here

	renderDevice.InitOpenGl(game.GetWorld().GetWidth(), game.GetWorld().GetHeight());

#pragma warning(push)
#pragma warning(disable : 4127)
	while (true) { // TODO remove always true
#pragma warning(pop) 
		game.Update();
		//<<<<<<< HEAD

		// For Testing Purposes Only...
		pos.x += Character["VelX"].Get<std::int32_t>();
		pos.y += Character["VelY"].Get<std::int32_t>();
		//pos.x += PlayerController.LeftStick.UnitMagnitudeX();
		//pos.y += PlayerController.LeftStick.UnitMagnitudeY();
		//pos.x += PlayerController.RightStick.UnitMagnitudeX();
		//pos.y += PlayerController.RightStick.UnitMagnitudeY();
		//pos.x -= PlayerController.LeftTrigger.UnitMagnitude();
		//pos.x += PlayerController.RightTrigger.UnitMagnitude();
		sprite.SetPosition(pos);
		//=======
		//>>>>>>> master
	}

	return 0;
}