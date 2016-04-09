#pragma once
#include "Entity.h"
#include "Factory.h"

class ActorEntity : public Library::Entity
{
	RTTI_DECLARATIONS(ActorEntity, Library::Entity);
public:
	ActorEntity();
	virtual ~ActorEntity() = default;

	virtual void Update(Library::WorldState& worldState) override;

private:
	void DefinePrescribedAttributes();
};

namespace Library
{
	CONCRETE_ENTITY_FACTORY(ActorEntity);
}
