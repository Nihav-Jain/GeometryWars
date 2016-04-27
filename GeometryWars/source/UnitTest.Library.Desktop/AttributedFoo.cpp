#include "pch.h"
#include "AttributedFoo.h"

using namespace Library;

RTTI_DEFINITIONS(AttributedFoo, Attributed);

AttributedFoo::AttributedFoo()
{
	mFooRtti = new FooRTTI();
	mScope = new Scope();

	mIntArray = new std::int32_t[10];
	mFloatArray = new std::float_t[10];
	mStringArray = new std::string[10];
	mVec4Array = new glm::vec4[10];
	mMat4Array = new glm::mat4[10];
	mRttiPtrArray = new RTTI*[10];

	DefinePrescribedAttributes();
}

AttributedFoo::AttributedFoo(const AttributedFoo& rhs)
{
	DeepCopyMemberVariables(rhs);
	DefinePrescribedAttributes();
}

AttributedFoo::AttributedFoo(AttributedFoo&& rhs) :
	mFooRtti(rhs.mFooRtti), mScope(rhs.mScope), mIntArray(rhs.mIntArray), mFloatArray(rhs.mFloatArray), mStringArray(rhs.mStringArray),
	mVec4Array(rhs.mVec4Array), mMat4Array(rhs.mMat4Array), mRttiPtrArray(rhs.mRttiPtrArray), Attributed(std::move(rhs))
{
	rhs.mFooRtti = nullptr;
	rhs.mScope = nullptr;

	rhs.mIntArray = nullptr;
	rhs.mFloatArray = nullptr;
	rhs.mStringArray = nullptr;
	rhs.mVec4Array = nullptr;
	rhs.mMat4Array = nullptr;
	rhs.mRttiPtrArray = nullptr;
}


AttributedFoo::~AttributedFoo()
{
	delete mFooRtti;
	delete mScope;

	delete[] mIntArray;
	delete[] mFloatArray;
	delete[] mStringArray;
	delete[] mVec4Array;
	delete[] mMat4Array;
	delete[] mRttiPtrArray;
}

AttributedFoo& AttributedFoo::operator=(const AttributedFoo& rhs)
{
	if (this != &rhs)
	{
		delete mFooRtti;
		delete mScope;

		delete[] mIntArray;
		delete[] mFloatArray;
		delete[] mStringArray;
		delete[] mVec4Array;
		delete[] mMat4Array;
		delete[] mRttiPtrArray;

		DeepCopyMemberVariables(rhs);
		Attributed::operator=(rhs);

		(*this)["mRtti"] = mFooRtti;

		(*this)["mExternalInt"].SetStorage(mIntArray, 10);
		(*this)["mExternalFloat"].SetStorage(mFloatArray, 10);
		(*this)["mExternalString"].SetStorage(mStringArray, 10);
		(*this)["mExternalVec4"].SetStorage(mVec4Array, 10);
		(*this)["mExternalMat4"].SetStorage(mMat4Array, 10);
		(*this)["mExternalRtti"].SetStorage(mRttiPtrArray, 10);
	}
	return *this;
}

AttributedFoo& AttributedFoo::operator=(AttributedFoo&& rhs)
{
	if (this != &rhs)
	{
		delete mFooRtti;
		delete mScope;

		delete[] mIntArray;
		delete[] mFloatArray;
		delete[] mStringArray;
		delete[] mVec4Array;
		delete[] mMat4Array;
		delete[] mRttiPtrArray;

		mFooRtti = (rhs.mFooRtti);
		mScope = (rhs.mScope);
		mIntArray = (rhs.mIntArray);
		mFloatArray = (rhs.mFloatArray);
		mStringArray = (rhs.mStringArray);
		mVec4Array = (rhs.mVec4Array);
		mMat4Array = (rhs.mMat4Array);
		mRttiPtrArray = (rhs.mRttiPtrArray);

		Attributed::operator=(std::move(rhs));

		(*this)["mRtti"] = mFooRtti;

		(*this)["mExternalInt"].SetStorage(mIntArray, 10);
		(*this)["mExternalFloat"].SetStorage(mFloatArray, 10);
		(*this)["mExternalString"].SetStorage(mStringArray, 10);
		(*this)["mExternalVec4"].SetStorage(mVec4Array, 10);
		(*this)["mExternalMat4"].SetStorage(mMat4Array, 10);
		(*this)["mExternalRtti"].SetStorage(mRttiPtrArray, 10);

		rhs.mFooRtti = nullptr;
		rhs.mScope = nullptr;

		rhs.mIntArray = nullptr;
		rhs.mFloatArray = nullptr;
		rhs.mStringArray = nullptr;
		rhs.mVec4Array = nullptr;
		rhs.mMat4Array = nullptr;
		rhs.mRttiPtrArray = nullptr;
	}
	return *this;
}

void AttributedFoo::DefinePrescribedAttributes()
{
	AddInternalAttribute("mInt", 10, 1);
	AddInternalAttribute("mFloat", 10.01f, 1);
	AddInternalAttribute("FooName", "Foo", 1);
	AddInternalAttribute("mVec4", glm::vec4(10, 20, 30, 40), 1);
	AddInternalAttribute("mMat4", glm::mat4(10), 1);
	AddInternalAttribute("mRtti", mFooRtti, 1);
	AddNestedScope("mScope", *mScope, 1);

	AddExternalAttribute("mExternalInt", 10, mIntArray);
	AddExternalAttribute("mExternalFloat", 10, mFloatArray);
	AddExternalAttribute("mExternalString", 10, mStringArray);
	AddExternalAttribute("mExternalVec4", 10, mVec4Array);
	AddExternalAttribute("mExternalMat4", 10, mMat4Array);
	AddExternalAttribute("mExternalRtti", 10, mRttiPtrArray);
}

void AttributedFoo::DeepCopyMemberVariables(const AttributedFoo& rhs)
{
	mFooRtti = new FooRTTI(*rhs.mFooRtti);
	mScope = new Scope(*rhs.mScope);

	mIntArray = new std::int32_t[10];
	mFloatArray = new std::float_t[10];
	mStringArray = new std::string[10];
	mVec4Array = new glm::vec4[10];
	mMat4Array = new glm::mat4[10];
	mRttiPtrArray = new RTTI*[10];

	for (std::uint32_t i = 0; i < 10; i++)
	{
		mIntArray[i] = rhs.mIntArray[i];
		mFloatArray[i] = rhs.mFloatArray[i];
		mStringArray[i] = rhs.mStringArray[i];
		mVec4Array[i] = rhs.mVec4Array[i];
		mMat4Array[i] = rhs.mMat4Array[i];
		mRttiPtrArray[i] = rhs.mRttiPtrArray[i];
	}
}
