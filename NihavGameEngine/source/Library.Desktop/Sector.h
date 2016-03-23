#pragma once
#include <string>
#include <sstream>

#include "Attributed.h"
#include "Entity.h"
#include "WorldState.h"

namespace Library
{
	class World;

	class Sector : public Attributed
	{
		RTTI_DECLARATIONS(Sector, Attributed);
	public:
		Sector();
		Sector(const Sector& rhs);
		Sector(Sector&& rhs);

		virtual ~Sector();

		Sector& operator=(const Sector& rhs);
		Sector& operator=(Sector&& rhs);

		const std::string& Name() const;
		void SetName(const std::string& name);

		Datum& Entities();
		Entity& CreateEntity(const std::string& entityClassName, const std::string& entityInstanceName);

		World* GetWorld() const;
		void SetWorld(World& parent);

		void Update(WorldState& worldState);

	protected:
		virtual void DefinePrescribedAttributes() override;

	private:
		std::string mName;

		const std::string ATTRIBUTE_NAME = "name";
		const std::string ATTRIBUTE_ENTITIES = "entities";
	};

}

