#include "pch.h"
#include "XmlParseHelperNameValue.h"

namespace Library
{
	Vector<std::string> XmlParseHelperNameValue::mAcceptableElementNames(2);

	XmlParseHelperNameValue::XmlParseHelperNameValue() :
		mCharData()
	{
		if (mAcceptableElementNames.IsEmpty())
		{
			mAcceptableElementNames.PushBack("name");
			mAcceptableElementNames.PushBack("value");
		}
	}


	XmlParseHelperNameValue::~XmlParseHelperNameValue()
	{
	}

	void XmlParseHelperNameValue::Initialize()
	{
		mCharData = std::string();
	}

	bool XmlParseHelperNameValue::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes)
	{
		UNREFERENCED_PARAMETER(attributes);

		XmlParseHelperTable::SharedDataTable* sharedDataPtr = sharedData.As<XmlParseHelperTable::SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (mAcceptableElementNames.Find(elementName) == mAcceptableElementNames.end())
			return false;

		if (elementName == "name")
		{
			if (!sharedDataPtr->CheckStateTransition(XmlParseHelperTable::SharedDataTable::ParserState::NAME_START))
				throw std::exception("Invalid script syntax");
			sharedDataPtr->ParsedElements.Push("name");
		}
		else if (elementName == "value")
		{
			if (!sharedDataPtr->CheckStateTransition(XmlParseHelperTable::SharedDataTable::ParserState::VALUE_START))
				throw std::exception("Invalid script syntax");
			sharedDataPtr->ParsedElements.Push("value");
		}


		return true;
	}

	bool XmlParseHelperNameValue::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName)
	{
		XmlParseHelperTable::SharedDataTable* sharedDataPtr = sharedData.As<XmlParseHelperTable::SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (mAcceptableElementNames.Find(elementName) == mAcceptableElementNames.end())
			return false;

		// assigne the name and the value here
		if (elementName == "name")
		{
			if (!sharedDataPtr->CheckStateTransition(XmlParseHelperTable::SharedDataTable::ParserState::NAME_END))
				throw std::exception("Invalid script syntax");
			sharedDataPtr->DataName = mCharData;
			sharedDataPtr->NameValueElementDataParsed = true;
			sharedDataPtr->ParsedElements.Pop();
		}
		else if (elementName == "value")
		{
			if (!sharedDataPtr->CheckStateTransition(XmlParseHelperTable::SharedDataTable::ParserState::VALUE_END))
				throw std::exception("Invalid script syntax");
			sharedDataPtr->DataValue = mCharData;
			sharedDataPtr->ParsedElements.Pop();
		}
		
		mCharData = "";
		return true;
	}

	bool XmlParseHelperNameValue::CharDataHandler(XmlParseMaster::SharedData& sharedData, const std::string& charData)
	{
		XmlParseHelperTable::SharedDataTable* sharedDataPtr = sharedData.As<XmlParseHelperTable::SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (*(sharedDataPtr->StateTraversor) != XmlParseHelperTable::SharedDataTable::ParserState::NAME_START && *(sharedDataPtr->StateTraversor) != XmlParseHelperTable::SharedDataTable::ParserState::VALUE_START)
			return false;

		mCharData.append(charData);
		return true;
	}

	IXmlParseHelper* XmlParseHelperNameValue::Clone() const
	{
		return new XmlParseHelperNameValue();
	}


}
