// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <cstdint>

#include <windows.h>
#include <exception>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <memory>
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <Shlwapi.h>
#include <WICTextureLoader.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#define DeleteObject(object) if((object) != nullptr) { delete object; object = nullptr; }
#define DeleteObjects(objects) if((objects) != nullptr) { delete[] objects; objects = nullptr; }
#define ReleaseObject(object) if((object) != nullptr) { object->Release(); object = nullptr; }

#include "GameException.h"

#pragma region SoundFMOD

#include <fstream>
#pragma warning(disable: 4505)
#include "fmod.hpp"
#include "fmod_errors.h"

#pragma endregion

namespace Library
{
	typedef unsigned char byte;
}

using namespace DirectX;
using namespace DirectX::PackedVector;