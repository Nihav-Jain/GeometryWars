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

		static World& CreateWorld(const std::string& name, Scope& parentScope);
		static const std::string ATTRIBUTE_NAME_SECTOR;

	protected:
		virtual void DefinePrescribedAttributes() override;

	private:
		std::string mName;
		WorldState mWorldState;

		static const std::string ATTRIBUTE_NAME_NAME;

	};
}

