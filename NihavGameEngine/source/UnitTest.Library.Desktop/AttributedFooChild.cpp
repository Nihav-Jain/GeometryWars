#include "pch.h"
#include "AttributedFooChild.h"

using namespace Library;
RTTI_DEFINITIONS(AttributedFooChild);

AttributedFooChild::AttributedFooChild()
{
	mRttiChild = new FooRTTI();
	Populate();
}

AttributedFooChild::AttributedFooChild(const AttributedFooChild& rhs)
{
	mRttiChild = new FooRTTI(*rhs.mRttiChild);
	Populate();
}


AttributedFooChild::~AttributedFooChild()
{
	delete mRttiChild;
}

void AttributedFooChild::DefinePrescribedAttributes()
{
	AddInternalAttribute("mIntChild", 10, 1);
	AddInternalAttribute("mFloatChild", 10.01f, 1);
	AddInternalAttribute("FooNameChild", "Foo", 1);
	AddInternalAttribute("mVec4Child", glm::vec4(10, 20, 30, 40), 1);
	AddInternalAttribute("mMat4Child", glm::mat4(10), 1);
	AddInternalAttribute("mRttiChild", mRttiChild, 1);
}