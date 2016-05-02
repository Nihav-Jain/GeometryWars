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

#include <iostream>
#include <fstream>

#include "RTTI.h"
#include "glm\glm.hpp"
#include <expat.h>

#pragma region Containers

#include "SList.h"
#include "Vector.h"
#include "Stack.h"
#include "Hashmap.h"
#include "Graph.h"

#pragma endregion

#include "Datum.h"
#include "Scope.h"
#include "Attributed.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"

#pragma region SoundFMOD

#include <fstream>
#pragma warning(disable: 4505)
#include "fmod.hpp"
#include "fmod_errors.h"
#include "AudioManager.h"

#pragma endregion

#include "GameObject.h"
