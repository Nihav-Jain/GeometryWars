// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// Headers for CppUnitTest
#include "CppUnitTest.h"

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <exception>
#include <iostream>
#include <cstdint>
#include <utility>
#include <string>

//#include "RTTI.h"
#include "SList.h"
#include "Vector.h"
#include "Stack.h"
#include "Hashmap.h"
#include "Graph.h"

#include "Datum.h"
#include "Scope.h"
#include "Attributed.h"

#include "Foo.h"
#include "FooRTTI.h"
#include "AttributedFoo.h"
#include "AttributedFooChild.h"
#include "AttributedNative.h"
#include "AudioManager.h"

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P)          (P)
#endif

