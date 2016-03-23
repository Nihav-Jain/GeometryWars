#include "pch.h"
#include "XmlParseHelperPrimitives.h"

namespace Library
{
	Hashmap<std::string, Datum::DatumType> XmlParseHelperPrimitives::mElementMetaData;

	XmlParseHelperPrimitives::XmlParseHelperPrimitives() :
		mCharData(), mCurrentDataName(), mStartState(SharedDataTable::ParserState::PRIMITIVE_START), mEndState(SharedDataTable::ParserState::PRIMITIVE_END)
	{
		if (mElementMetaData.Size() == 0U)
		{
			mElementMetaData["integer"] = Datum::DatumType::INTEGER;
			mElementMetaData["float"] = Datum::DatumType::FLOAT;
			mElementMetaData["string"] = Datum::DatumType::STRING;
			mElementMetaData["vector"] = Datum::DatumType::VECTOR4;
			mElementMetaData["matrix"] = Datum::DatumType::MATRIX4x4;
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
		if (!mElementMetaData.ContainsKey(elementName))
			return false;

		if (!sharedDataPtr->CheckStateTransition(mStartState))
			throw std::exception("Invalid script syntax");

		// <integer name="variableName">
		if (attributes.ContainsKey("name"))
		{
			if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::NAME_START))
				throw std::exception("Invalid script syntax");
			if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::NAME_END))
				throw std::exception("Invalid script syntax");

			mCurrentDataName = attributes["name"];
			Datum& primitiveDatum = sharedDataPtr->CurrentScopePtr->Append(mCurrentDataName);
			primitiveDatum.SetType(mElementMetaData[elementName]);

			// <integer name="variableName" value="variableValue"/>
			if (attributes.ContainsKey("value"))
			{
				if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::VALUE_START))
					throw std::exception("Invalid script syntax");
				if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::VALUE_END))
					throw std::exception("Invalid script syntax");

				primitiveDatum.SetFromString(attributes["value"], primitiveDatum.Size());
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
		if (!mElementMetaData.ContainsKey(elementName))
			return false;

		// <integer name="varname">10</integer>
		if (!mCharData.empty())
		{
			if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::VALUE_END))
				throw std::exception("Invalid script syntax");
			Datum& datum = sharedDataPtr->CurrentScopePtr->operator[](mCurrentDataName);
			datum.SetFromString(mCharData, datum.Size());
		}

		// <integer>
		//		<name>varname</name>
		//		<value>10</value>
		// </integer>
		else if (sharedDataPtr->NameValueElementDataParsed)
		{
			Datum& primitiveDatum = sharedDataPtr->CurrentScopePtr->Append(sharedDataPtr->DataName);
			primitiveDatum.SetType(mElementMetaData[elementName]);
			primitiveDatum.SetFromString(sharedDataPtr->DataValue, primitiveDatum.Size());
		}

		if (!sharedDataPtr->CheckStateTransition(mEndState))
			throw std::exception("Invalid script syntax");
		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::END_STATE_ROUTER))
			throw std::exception("Invalid script syntax");

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
		if (mCharData.empty())
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

	void XmlParseHelperPrimitives::ClearStaticMembers()
	{
		mElementMetaData.Clear();
	}

}
