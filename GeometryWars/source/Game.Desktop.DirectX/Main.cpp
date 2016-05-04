#include "pch.h"
#include "Game.h"
//#include "PolygonRenderer.h"
#include "ReactionAttributed.h"
#include "InputManager.h"
#include "ActionDebug.h"
#include "D3DRenderDevice.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "EnemySquare.h"
#include "Collectible.h"
#include "BloomPostProcessing.h"


#include <fstream>

using namespace Library;


void InitializeWindow(HINSTANCE instance, const std::wstring& className, const std::wstring windowTitle, int showCommand);
LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
POINT CenterWindow(int windowWidth, int windowHeight);
void Shutdown(const std::wstring& className);


UINT mScreenWidth = 800;
UINT mScreenHeight = 600;

HWND mWindowHandle;
WNDCLASSEX mWindow;
using namespace Library;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{
	UNREFERENCED_PARAMETER(instance);
	UNREFERENCED_PARAMETER(previousInstance);
	UNREFERENCED_PARAMETER(commandLine);
	UNREFERENCED_PARAMETER(showCommand);

	XBoxControllerHandlerFactory xchf;
	ActionDebugFactory adf;
	PlayerFactory mPlayerFactory;
	EnemyFactory mEnemyFactory;
	EnemySquareFactory mEnemySquareFactory;
	BulletFactory mBulletFactory;
	CollectibleFactory mCollectibleFactory;

	BloomPostProcessing bloom;
	std::wstring windowClassName = L"RenderingClass";


	WCHAR buffer[MAX_PATH];
	GetModuleFileName(nullptr, buffer, MAX_PATH);
	PathRemoveFileSpec(buffer);
	SetCurrentDirectory(std::wstring(buffer).c_str());

	D3DRenderDevice device;
	Renderer * renderer = Renderer::GetInstance(&device);

	Game game;
	game.SetRenderer(renderer);
	game.Start("Content/config/geometrywars_test.xml");

	mScreenWidth = game.GetWorld().GetWidth();
	mScreenHeight = game.GetWorld().GetHeight();
	InitializeWindow(instance, windowClassName, L"Geometry Wars DirectX", showCommand);
	MSG message;
	ZeroMemory(&message, sizeof(message));
	device.InitializeDirectX(mWindowHandle, mScreenWidth, mScreenHeight);

	//game.Start("Content/config/geometrywars_test.xml");
	while (message.message != WM_QUIT)
	{
		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			game.Update();
		}
	}

	Shutdown(windowClassName);
	return 0;
}

void InitializeWindow(HINSTANCE instance, const std::wstring& className, const std::wstring windowTitle, int showCommand)
{
	ZeroMemory(&mWindow, sizeof(mWindow));
	mWindow.cbSize = sizeof(WNDCLASSEX);
	mWindow.style = CS_CLASSDC;
	mWindow.lpfnWndProc = WndProc;
	mWindow.hInstance = instance;
	mWindow.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	mWindow.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	mWindow.hCursor = LoadCursor(nullptr, IDC_ARROW);
	mWindow.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
	mWindow.lpszClassName = className.c_str();

	RECT windowRectangle = { 0, 0, (LONG)mScreenWidth, (LONG)mScreenHeight };
	AdjustWindowRect(&windowRectangle, WS_OVERLAPPEDWINDOW, FALSE);

	RegisterClassEx(&mWindow);
	POINT center = CenterWindow(mScreenWidth, mScreenHeight);
	mWindowHandle = CreateWindow(className.c_str(), windowTitle.c_str(), WS_OVERLAPPEDWINDOW, center.x, center.y, windowRectangle.right - windowRectangle.left, windowRectangle.bottom - windowRectangle.top, nullptr, nullptr, instance, nullptr);

	ShowWindow(mWindowHandle, showCommand);
	UpdateWindow(mWindowHandle);
}

LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(windowHandle, message, wParam, lParam);
}

POINT CenterWindow(int windowWidth, int windowHeight)
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	POINT center;
	center.x = (screenWidth - windowWidth) / 2;
	center.y = (screenHeight - windowHeight) / 2;

	return center;
}

void Shutdown(const std::wstring& className)
{
//	ReleaseObject(mRenderTargetView);
//	ReleaseObject(mDepthStencilView);
//	ReleaseObject(mSwapChain);
//	ReleaseObject(mDepthStencilBuffer);

//	if (mDirect3DDeviceContext != nullptr)
//	{
//		mDirect3DDeviceContext->ClearState();
//	}
//
//	ReleaseObject(mDirect3DDeviceContext);
//	ReleaseObject(mDirect3DDevice);

	UnregisterClass(className.c_str(), mWindow.hInstance);
}
