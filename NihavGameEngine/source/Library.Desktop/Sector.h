#pragma once
#include "Attributed.h"

namespace Library
{
	class Sector : public Attributed
	{
		RTTI_DECLARATIONS(Sector, Attributed);
	public:
		Sector();
		virtual ~Sector();
	};

}

