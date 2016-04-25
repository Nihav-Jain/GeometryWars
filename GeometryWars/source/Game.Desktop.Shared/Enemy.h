#pragma once
#include "../../source/Library.Desktop/GameObject.h"

namespace Library
{
	class Enemy : public GameObject
	{
		RTTI_DECLARATIONS(Enemy, GameObject);

	public:

		Enemy();
		virtual ~Enemy() = default;


	protected:

		// TODO: What member variables does every enemy need?

	};

	CONCRETE_ENTITY_FACTORY(Enemy);
}

