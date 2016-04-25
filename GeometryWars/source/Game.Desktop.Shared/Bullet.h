#pragma once
#include "../../source/Library.Desktop/GameObject.h"

namespace Library
{
	class Bullet : public GameObject
	{
		RTTI_DECLARATIONS(Bullet, GameObject);

	public:

		Bullet();
		virtual ~Bullet() = default;

	};

	CONCRETE_ENTITY_FACTORY(Bullet);
}
