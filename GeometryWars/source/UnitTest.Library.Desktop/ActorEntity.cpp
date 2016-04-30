#include "pch.h"
#include "ActorEntity.h"

using namespace Library;

RTTI_DEFINITIONS(ActorEntity, Entity);

ActorEntity::ActorEntity()
{
	DefinePrescribedAttributes();
}

void ActorEntity::Update(Library::WorldState& worldState)
{
	Entity::Update(worldState);

	// do work specific to this Entity
	(*this)["position"].Get<glm::vec4>().x += 10;
}

void ActorEntity::DefinePrescribedAttributes()
{
	AddInternalAttribute("position", glm::vec4(0), 1);
}
