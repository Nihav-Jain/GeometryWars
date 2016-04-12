#pragma once
#include "Attributed.h"

class AttributedNative : public Library::Attributed
{
	RTTI_DECLARATIONS(AttributedNative, Library::Attributed);
public:
	AttributedNative();
	AttributedNative(const AttributedNative& rhs);
	AttributedNative(AttributedNative&& rhs);

	virtual ~AttributedNative() = default;

	AttributedNative& operator=(const AttributedNative& rhs);
	AttributedNative& operator=(AttributedNative&& rhs);

protected:
	void DefinePrescribedAttributes();
};

