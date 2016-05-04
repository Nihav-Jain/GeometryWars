#include "pch.h"
#include "XmlParseHelperNameValue.h"

namespace Library
{
	XmlParseHelperNameValue::XmlParseHelperNameValue() :
		mCharData()
	{}

	void XmlParseHelperNameValue::Initialize()
	{
		mCharData = std::string();
	}

	bool XmlParseHelperNameValue::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes)
	{
		UNREFERENCED_PARAMETER(attributes);

		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;

		if (elementName == "name")
		{
			if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::NAME_START))
				throw std::exception("ClearScreen script syntax");
		}
		else if (elementName == "value")
		{
			if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::VALUE_START))
				throw std::exception("ClearScreen script syntax");
		}
		else
		{
			return false;
		}


		return true;
	}

	bool XmlParseHelperNameValue::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;

		if (elementName == "name")
		{
			if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::NAME_END))
				throw std::exception("ClearScreen script syntax");
			sharedDataPtr->DataName = mCharData;
			sharedDataPtr->NameValueElementDataParsed = true;
		}
		else if (elementName == "value")
		{
			if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::VALUE_END))
				throw std::exception("ClearScreen script syntax");
			sharedDataPtr->DataValue = mCharData;
		}
		else
		{
			return false;
		}
		
		mCharData = "";
		return true;
	}

	bool XmlParseHelperNameValue::CharDataHandler(XmlParseMaster::SharedData& sharedData, const std::string& charData)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (*(sharedDataPtr->StateTraversor()) != SharedDataTable::ParserState::NAME_START && *(sharedDataPtr->StateTraversor()) != SharedDataTable::ParserState::VALUE_START)
			return false;

		mCharData.append(charData);
		return true;
	}

	IXmlParseHelper* XmlParseHelperNameValue::Clone() const
	{
		return new XmlParseHelperNameValue();
	}

}
