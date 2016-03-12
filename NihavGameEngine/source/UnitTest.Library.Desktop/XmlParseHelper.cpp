#include "pch.h"
#include "XmlParseHelper.h"
#include "TestSharedData.h"

using namespace Library;

void XmlParseHelper::Initialize()
{}

bool XmlParseHelper::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes)
{
	UNREFERENCED_PARAMETER(sharedData);
	UNREFERENCED_PARAMETER(elementName);
	UNREFERENCED_PARAMETER(attributes);
	
	return true;
}

bool XmlParseHelper::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName)
{
	UNREFERENCED_PARAMETER(sharedData);
	UNREFERENCED_PARAMETER(elementName);

	return true;
}

bool XmlParseHelper::CharDataHandler(Library::XmlParseMaster::SharedData& sharedData, const std::string& charData)
{
	TestSharedData* data = sharedData.As<TestSharedData>();
	if (data == nullptr)
		return false;
	
	data->mData = charData;

	return true;
}

IXmlParseHelper* XmlParseHelper::Clone() const
{
	return new XmlParseHelper();
}
