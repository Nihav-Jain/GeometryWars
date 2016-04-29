#include "pch.h"
#include "AttributedExternalError.h"

using namespace Library;

RTTI_DEFINITIONS(AttributedExternalError, Attributed);

AttributedExternalError::AttributedExternalError()
{
	mIntArray = new std::int32_t[10];
	AddExternalAttribute("mIntArray", 10, mIntArray);
	try
	{
		AddExternalAttribute("mIntArray", 10, mIntArray);
	}
	catch (const std::exception& e)
	{
		UNREFERENCED_PARAMETER(e);
		delete[] mIntArray;
		throw;
	}
}

AttributedExternalError::~AttributedExternalError()
{
	delete[] mIntArray;
}



