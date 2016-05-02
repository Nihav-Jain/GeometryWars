#include "pch.h"
#include "AttributedInternalError.h"

using namespace Library;

RTTI_DEFINITIONS(AttributedInternalError, Attributed);

AttributedInternalError::AttributedInternalError()
{
	DefinePrescribedAttributes();
}

void AttributedInternalError::DefinePrescribedAttributes()
{
	AddInternalAttribute("mNativeInt", 10, 1);
	AddInternalAttribute("mNativeFloat", 10.01f, 1);
	AddInternalAttribute("mNativeString", "Foo", 1);
	AddInternalAttribute("mNativeInt", 20, 1);
}


