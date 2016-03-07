#pragma once
#include "Attributed.h"

class AttributedInternalError : public Library::Attributed
{
	RTTI_DECLARATIONS(AttributedInternalError, Library::Attributed);
public:
	AttributedInternalError();
	virtual ~AttributedInternalError() = default;


protected:
	virtual void DefinePrescribedAttributes() override;
};

