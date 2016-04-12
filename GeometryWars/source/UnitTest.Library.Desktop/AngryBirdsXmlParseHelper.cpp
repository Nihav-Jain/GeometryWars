#include "pch.h"
#include "AngryBirdsXmlParseHelper.h"
#include "AngryBirdsSharedData.h"

using namespace Library;
Hashmap<std::string, AngryBirdsXmlParseHelper::ElementParser> AngryBirdsXmlParseHelper::mElementParsers;

AngryBirdsXmlParseHelper::AngryBirdsXmlParseHelper() :
	//mElementParsers(), mElementStack()
	mElementStack()
{
	mElementParsers["angrybirds"] = &AngryBirdsXmlParseHelper::ParseElement<ElementNames::AngryBirds>;
	mElementParsers["birds"] = &AngryBirdsXmlParseHelper::ParseElement<ElementNames::Birds>;
}

AngryBirdsXmlParseHelper::AngryBirdsXmlParseHelper(const AngryBirdsXmlParseHelper& rhs) :
	//mElementParsers(rhs.mElementParsers), mElementStack(rhs.mElementStack)
	mElementStack(rhs.mElementStack)
{}

AngryBirdsXmlParseHelper::AngryBirdsXmlParseHelper(AngryBirdsXmlParseHelper&& rhs) :
	//mElementParsers(std::move(rhs.mElementParsers)), mElementStack(std::move(rhs.mElementStack))
	mElementStack(std::move(rhs.mElementStack))
{}

AngryBirdsXmlParseHelper& AngryBirdsXmlParseHelper::operator=(const AngryBirdsXmlParseHelper& rhs)
{
	if (this != &rhs)
	{
		mElementParsers = rhs.mElementParsers;
		mElementStack = rhs.mElementStack;
	}
	return *this;
}

AngryBirdsXmlParseHelper& AngryBirdsXmlParseHelper::operator=(AngryBirdsXmlParseHelper&& rhs)
{
	if (this != &rhs)
	{
		mElementParsers = std::move(rhs.mElementParsers);
		mElementStack = std::move(rhs.mElementStack);
	}
	return *this;
}

void AngryBirdsXmlParseHelper::Initialize()
{
	while (!mElementStack.IsEmpty())
		mElementStack.Pop();
}

bool AngryBirdsXmlParseHelper::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes)
{
	AngryBirdsSharedData* angryBirdsData = sharedData.As<AngryBirdsSharedData>();
	if (angryBirdsData == nullptr)
		return false;
	if (!mElementParsers.ContainsKey(elementName))
		return false;

	mElementStack.Push(elementName);

	return (this->*mElementParsers[elementName])(*angryBirdsData, attributes);
}

bool AngryBirdsXmlParseHelper::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName)
{
	AngryBirdsSharedData* angryBirdsData = sharedData.As<AngryBirdsSharedData>();
	if (angryBirdsData == nullptr)
		return false;
	if (!mElementParsers.ContainsKey(elementName))
		return false;
	assert(mElementStack.Top() == elementName);
	mElementStack.Pop();
	return true;
}

IXmlParseHelper* AngryBirdsXmlParseHelper::Clone() const
{
	return new AngryBirdsXmlParseHelper();
}

void AngryBirdsXmlParseHelper::ClearElementParsers()
{
	mElementParsers.Clear();
}

#pragma region ParseElement

template <>
bool AngryBirdsXmlParseHelper::ParseElement<AngryBirdsXmlParseHelper::ElementNames::AngryBirds>(AngryBirdsSharedData& sharedData, const Hashmap<std::string, std::string>& attributes)
{
	UNREFERENCED_PARAMETER(attributes);

	sharedData.mAngryBirdsData.AppendScope("angrybirds");
	return true;
}

template <>
bool AngryBirdsXmlParseHelper::ParseElement<AngryBirdsXmlParseHelper::ElementNames::Birds>(AngryBirdsSharedData& sharedData, const Hashmap<std::string, std::string>& attributes)
{
	Datum& angrybirds = sharedData.mAngryBirdsData["angrybirds"];
	Scope& birds = angrybirds.Get<Scope>().AppendScope("birds");
	
	birds["src"] = attributes["src"];
	birds["imageWidth"] = std::stoi(attributes["imageWidth"]);
	birds["imageHeight"] = std::stoi(attributes["imageHeight"]);

	return true;
}

#pragma endregion