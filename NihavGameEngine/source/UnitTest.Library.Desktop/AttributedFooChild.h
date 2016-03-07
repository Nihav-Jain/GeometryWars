#pragma once
#include "AttributedFoo.h"

class AttributedFooChild : public AttributedFoo
{
	RTTI_DECLARATIONS(AttributedFooChild, AttributedFoo);
public:
	AttributedFooChild();
	AttributedFooChild(const AttributedFooChild& rhs);
	virtual ~AttributedFooChild();

protected:
	virtual void DefinePrescribedAttributes() override;

private:
	FooRTTI* mRttiChild;
};

