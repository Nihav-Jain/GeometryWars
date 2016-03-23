#pragma once
#include "RTTI.h"
#include "Attributed.h"
#include "Factory.h"
#include "WorldState.h"

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

		const std::string& Name() const;
		void SetName(const std::string& name);

		Sector* GetSector() const;
		void SetSector(Sector& parent);

		void Update(WorldState& worldState);

	protected:
		virtual void DefinePrescribedAttributes() override;

	private:
		std::string mName;
		const std::string ATTRIBUTE_NAME = "name";
	};

	CONCRETE_FACTORY(Entity, RTTI);
}

