#pragma once
#include "Attributed.h"

namespace Library
{
	class World : public Attributed
	{
		RTTI_DECLARATIONS(World, Attributed);
	public:
		World();
		virtual ~World();
	};
}

