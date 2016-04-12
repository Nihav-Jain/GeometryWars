#pragma once
#include "AttributedFoo.h"

class AttributedFooChild : public AttributedFoo
{
	RTTI_DECLARATIONS(AttributedFooChild, AttributedFoo);
public:
	AttributedFooChild();
	AttributedFooChild(const AttributedFooChild& rhs);
	virtual ~AttributedFooChild();

private:
	void DefinePrescribedAttributes();
	FooRTTI* mRttiChild;
};

