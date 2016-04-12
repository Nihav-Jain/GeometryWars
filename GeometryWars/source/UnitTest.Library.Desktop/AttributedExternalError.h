#pragma once
#include "Attributed.h"

class AttributedExternalError : public Library::Attributed
{
	RTTI_DECLARATIONS(AttributedExternalError, Library::Attributed);
public:
	AttributedExternalError();
	virtual ~AttributedExternalError();


private:
	std::int32_t* mIntArray;
};

