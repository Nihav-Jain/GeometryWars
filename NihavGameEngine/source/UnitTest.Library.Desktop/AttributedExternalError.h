#pragma once
#include "Attributed.h"

class AttributedExternalError : public Library::Attributed
{
	RTTI_DECLARATIONS(AttributedExternalError, Library::Attributed);
public:
	AttributedExternalError();
	virtual ~AttributedExternalError();


protected:
	virtual void DefinePrescribedAttributes() override;

private:
	std::int32_t* mIntArray;
};

