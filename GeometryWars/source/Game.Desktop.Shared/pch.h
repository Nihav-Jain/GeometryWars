// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P)          (P)
#endif

#include <exception>
#include <cstdint>
#include <utility>
#include <string>

#include "../../source/Library.Desktop/RTTI.h"
#include "glm\glm.hpp"

#pragma region Containers

#include "../../source/Library.Desktop/SList.h"
#include "../../source/Library.Desktop/Vector.h"
#include "../../source/Library.Desktop/Stack.h"
#include "../../source/Library.Desktop/Hashmap.h"
#include "../../source/Library.Desktop/Graph.h"

#pragma endregion

#include "../../source/Library.Desktop/Datum.h"
#include "../../source/Library.Desktop/Scope.h"
#include "../../source/Library.Desktop/Attributed.h"
#include "../../source/Library.Desktop/Entity.h"
#include "../../source/Library.Desktop/Sector.h"
#include "../../source/Library.Desktop/World.h"
#include "../../source/Library.Desktop/GameObject.h"

#include "SpriteManager.h"
#include "ScoreManager.h"
#include "LivesManager.h"
#include "BombManager.h"

#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"