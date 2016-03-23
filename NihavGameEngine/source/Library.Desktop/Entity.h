#pragma once
#include "Attributed.h"

namespace Library
{
	class Sector;

	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed);
	public:
		Entity();
		Entity(const Entity& rhs);
		Entity(Entity&& rhs);

		virtual ~Entity();

		Entity& operator=(const Entity& rhs);
		Entity& operator=(Entity&& rhs);

		std::string Name() const;
		void SetName(const std::string& name);

		Sector* GetSector() const;
		void SetSector(Sector& parent);

		void Update();

	protected:
		virtual void DefinePrescribedAttributes() override;

	private:
		std::string mName;
	};

}

