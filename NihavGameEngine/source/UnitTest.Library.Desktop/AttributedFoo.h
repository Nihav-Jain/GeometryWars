#pragma once
#include "Attributed.h"

class AttributedFoo : public Library::Attributed
{
	RTTI_DECLARATIONS(AttributedFoo, Library::Attributed);
public:
	AttributedFoo();
	AttributedFoo(const AttributedFoo& rhs);
	AttributedFoo(AttributedFoo&& rhs);
	virtual ~AttributedFoo();

	AttributedFoo& operator=(const AttributedFoo& rhs);
	AttributedFoo& operator=(AttributedFoo&& rhs);

protected:
	virtual void DefinePrescribedAttributes() override;

private:
	void DeepCopyMemberVariables(const AttributedFoo& rhs);

	FooRTTI* mFooRtti;
	Scope* mScope;

	std::int32_t* mIntArray;
	std::float_t* mFloatArray;
	std::string* mStringArray;
	glm::vec4* mVec4Array;
	glm::mat4* mMat4Array;
	RTTI** mRttiPtrArray;
};

