#include "pch.h"
#include "AttributedNative.h"

using namespace Library;

RTTI_DEFINITIONS(AttributedNative, Attributed);

AttributedNative::AttributedNative()
{
	DefinePrescribedAttributes();
}

AttributedNative::AttributedNative(const AttributedNative& rhs) :
	Attributed(rhs)
{}

AttributedNative::AttributedNative(AttributedNative&& rhs) :
	Attributed::Attributed(std::move(rhs))
{}

AttributedNative& AttributedNative::operator=(const AttributedNative& rhs)
{
	if (this != &rhs)
	{
		Attributed::operator=(rhs);
	}
	return *this;
}

AttributedNative& AttributedNative::operator=(AttributedNative&& rhs)
{
	if (this != &rhs)
	{
		Attributed::operator=(std::move(rhs));
	}
	return *this;
}

void AttributedNative::DefinePrescribedAttributes()
{
	AddInternalAttribute("mNativeInt", 10, 1);
	AddInternalAttribute("mNativeFloat", 10.01f, 1);
	AddInternalAttribute("mNativeString", "Foo", 1);
}


