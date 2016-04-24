#pragma once
#include "Entity.h"

class SampleEntity : public Library::Entity
{
	RTTI_DECLARATIONS(SampleEntity, Library::Entity);
public:
	SampleEntity();
	virtual ~SampleEntity() = default;
};

namespace Library
{
	CONCRETE_ENTITY_FACTORY(SampleEntity);
}
