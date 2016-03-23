#pragma once
#include "Attributed.h"
#include "Sector.h"
#include "WorldState.h"

namespace Library
{
	class World : public Attributed
	{
		RTTI_DECLARATIONS(World, Attributed);
	public:
		World();
		World(const World& rhs);
		World(World&& rhs);

		virtual ~World();

		World& operator=(const World& rhs);
		World& operator=(World&& rhs);

		const std::string& Name() const;
		void SetName(const std::string& name);

		Datum& Sectors();
		Sector& CreateSector(const std::string& sectorName);

		void Update();

	protected:
		virtual void DefinePrescribedAttributes() override;

	private:
		std::string mName;
		WorldState mWorldState;

		const std::string ATTRIBUTE_NAME_NAME = "name";
		const std::string ATTRIBUTE_NAME_SECTOR = "sectors";
	};
}

