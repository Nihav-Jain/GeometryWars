#include "pch.h"
#include "Game.h"
#include "ReactionAttributed.h"
#include "InputManager.h"
#include "TestClass.h"

using namespace Library;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{
	UNREFERENCED_PARAMETER(instance);
	UNREFERENCED_PARAMETER(previousInstance);
	UNREFERENCED_PARAMETER(commandLine);
	UNREFERENCED_PARAMETER(showCommand);

	TestClass c;
	UNREFERENCED_PARAMETER(c);

	const glm::vec4 CornflowerBlue = glm::vec4(0.392f, 0.584f, 0.929f, 1.0f);

	if (!glfwInit())
	{
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Essentials", nullptr, nullptr);
	if (window == nullptr)
	{
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (gl3wInit() != 0)
	{
		return -1;
	}

	Game game;
	EntityFactory entityFactory;
	ActionListFactory actionListFactory;
	ActionListSwitchFactory switchFactory;
	ActionListSwitch::ActionListSwitchCaseFactory switchCaseFactory;
	ActionExpressionFactory expFactory;
	ReactionAttributedFactory reactionAttributedFactory;

	KeyBoardHandlerFactory khf;
	XBoxControllerHandlerFactory xchf;

	OutputDebugString(L"LOOK AT ME!");
	OutputDebugStringA("HELPOPO AT ME!");
	OutputDebugStringW(L"WORLD AT ME!");
	game.ParseMaster().ParseFromFile("../../Content/config/input.xml");
	game.Start();

	glViewport(0, 0, 800, 600);

	while (!glfwWindowShouldClose(window))
	{
		glClearBufferfv(GL_COLOR, 0, &CornflowerBlue[0]);

		glfwSwapBuffers(window);
		glfwPollEvents();

		game.Update();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}