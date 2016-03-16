#include "pch.h"
#include "XmlParseHelperPrimitives.h"

namespace Library
{
	Hashmap<std::string, XmlParseHelperPrimitives::ValueConvertor> XmlParseHelperPrimitives::mValueConvertors;
	Hashmap<std::string, SharedDataTable::ParserState> XmlParseHelperPrimitives::mElementParseEndStates;
	Hashmap<std::string, SharedDataTable::ParserState> XmlParseHelperPrimitives::mElementParseStartStates;

	XmlParseHelperPrimitives::XmlParseHelperPrimitives() :
		mCharData(), mCurrentDataName()
	{
		if (mValueConvertors.Size() == 0U)
		{
			mValueConvertors["integer"] = &XmlParseHelperPrimitives::ConvertValue<SharedDataTable::ParserState::INTEGER_END>;
			mValueConvertors["float"] = &XmlParseHelperPrimitives::ConvertValue<SharedDataTable::ParserState::FLOAT_END>;
			mValueConvertors["string"] = &XmlParseHelperPrimitives::ConvertValue<SharedDataTable::ParserState::STRING_END>;

			mElementParseEndStates["integer"] = SharedDataTable::ParserState::INTEGER_END;
			mElementParseEndStates["float"] = SharedDataTable::ParserState::FLOAT_END;
			mElementParseEndStates["string"] = SharedDataTable::ParserState::STRING_END;

			mElementParseStartStates["integer"] = SharedDataTable::ParserState::INTEGER_START;
			mElementParseStartStates["float"] = SharedDataTable::ParserState::FLOAT_START;
			mElementParseStartStates["string"] = SharedDataTable::ParserState::STRING_START;
		}
	}

	void XmlParseHelperPrimitives::Initialize()
	{
		mCharData = "";
	}

	bool XmlParseHelperPrimitives::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (!mValueConvertors.ContainsKey(elementName))
			return false;

		if (!sharedDataPtr->CheckStateTransition(mElementParseStartStates[elementName]))
			throw std::exception("Invalid script syntax");
		sharedDataPtr->ParsedElements.Push(elementName);
		// <integer name="variableName">
		if (attributes.ContainsKey("name"))
		{
			if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::NAME_START))
				throw std::exception("Invalid script syntax");
			if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::NAME_END))
				throw std::exception("Invalid script syntax");

			mCurrentDataName = attributes["name"];
			Datum& primitiveDatum = sharedDataPtr->ScopeStack.Top()->Append(mCurrentDataName);

			if (mCurrentDataName == "integer")
				primitiveDatum = 0;
			else if (mCurrentDataName == "float")
				primitiveDatum = 0.0f;
			else if (mCurrentDataName == "string")
				primitiveDatum = "";

			// <integer name="variableName" value="variableValue"/>
			if (attributes.ContainsKey("value"))
			{
				if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::VALUE_START))
					throw std::exception("Invalid script syntax");
				if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::VALUE_END))
					throw std::exception("Invalid script syntax");

				(this->*mValueConvertors[elementName])(*sharedDataPtr, mCurrentDataName, attributes["value"]);
				mCurrentDataName = "";
			}
		}

		return true;
	}

	bool XmlParseHelperPrimitives::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;		
		if (!mValueConvertors.ContainsKey(elementName))
			return false;

		// <integer name="varname">10</integer>
		if (!mCharData.empty())
		{
			if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::VALUE_END))
				throw std::exception("Invalid script syntax");
			(this->*mValueConvertors[elementName])(*sharedDataPtr, mCurrentDataName, mCharData);
		}
		// <integer>
		//		<name>varname</name>
		//		<value>10</value>
		// </integer>
		else if (sharedDataPtr->NameValueElementDataParsed)
		{
			(this->*mValueConvertors[elementName])(*sharedDataPtr, sharedDataPtr->DataName, sharedDataPtr->DataValue);
		}

		if (!sharedDataPtr->CheckStateTransition(mElementParseEndStates[elementName]))
			throw std::exception("Invalid script syntax");
		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::END_STATE_ROUTER))
			throw std::exception("Invalid script syntax");

		sharedDataPtr->ParsedElements.Pop();
		sharedDataPtr->NameValueElementDataParsed = false;
		mCurrentDataName = "";
		mCharData = "";
		return true;
	}

	bool XmlParseHelperPrimitives::CharDataHandler(XmlParseMaster::SharedData& sharedData, const std::string& charData)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (!mValueConvertors.ContainsKey(sharedDataPtr->ParsedElements.Top()))
			return false;
		if (mCharData.empty() && !charData.empty())
		{
			if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::VALUE_START))
				return false;
		}
		mCharData.append(charData);
		return true;
	}

	IXmlParseHelper* XmlParseHelperPrimitives::Clone() const
	{
		return new XmlParseHelperPrimitives();
	}

#pragma region ValueConvertors

	template<>
	void XmlParseHelperPrimitives::ConvertValue<SharedDataTable::ParserState::INTEGER_END>(SharedDataTable& sharedData, const std::string& name, const std::string& valueStr)
	{
		sharedData.ScopeStack.Top()->operator[](name) = std::stoi(valueStr);
	}

	template<>
	void XmlParseHelperPrimitives::ConvertValue<SharedDataTable::ParserState::FLOAT_END>(SharedDataTable& sharedData, const std::string& name, const std::string& valueStr)
	{
		sharedData.ScopeStack.Top()->operator[](name) = std::stof(valueStr);
	}

	template<>
	void XmlParseHelperPrimitives::ConvertValue<SharedDataTable::ParserState::STRING_END>(SharedDataTable& sharedData, const std::string& name, const std::string& valueStr)
	{
		sharedData.ScopeStack.Top()->operator[](name) = valueStr;
	}

#pragma endregion
}
