#include "pch.h"
#include "XmlParseHelperPrimitives.h"

namespace Library
{
	Hashmap<std::string, XmlParseHelperPrimitives::MetaData> XmlParseHelperPrimitives::mElementMetaData;

	XmlParseHelperPrimitives::XmlParseHelperPrimitives() :
		mCharData(), mCurrentDataName()
	{
		if (mElementMetaData.Size() == 0U)
		{
			mElementMetaData["integer"].mType = Datum::DatumType::INTEGER;
			mElementMetaData["integer"].mStartState = SharedDataTable::ParserState::INTEGER_START;
			mElementMetaData["integer"].mEndState = SharedDataTable::ParserState::INTEGER_END;

			mElementMetaData["float"].mType = Datum::DatumType::FLOAT;
			mElementMetaData["float"].mStartState = SharedDataTable::ParserState::FLOAT_START;
			mElementMetaData["float"].mEndState = SharedDataTable::ParserState::FLOAT_END;

			mElementMetaData["string"].mType = Datum::DatumType::STRING;
			mElementMetaData["string"].mStartState = SharedDataTable::ParserState::STRING_START;
			mElementMetaData["string"].mEndState = SharedDataTable::ParserState::STRING_END;

			mElementMetaData["vector"].mType = Datum::DatumType::VECTOR4;
			mElementMetaData["vector"].mStartState = SharedDataTable::ParserState::VECTOR_START;
			mElementMetaData["vector"].mEndState = SharedDataTable::ParserState::VECTOR_END;

			mElementMetaData["matrix"].mType = Datum::DatumType::MATRIX4x4;
			mElementMetaData["matrix"].mStartState = SharedDataTable::ParserState::MATRIX_START;
			mElementMetaData["matrix"].mEndState = SharedDataTable::ParserState::MATRIX_END;
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

		if (!sharedDataPtr->CheckStateTransition(mElementMetaData[elementName].mStartState))
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
			primitiveDatum.SetType(mElementMetaData[elementName].mType);
			std::uint32_t currentDatumSize = primitiveDatum.Size();

			if (elementName == "integer")
				primitiveDatum.Set(0, currentDatumSize);
			else if (elementName == "float")
				primitiveDatum.Set(0.0f, currentDatumSize);
			else if (elementName == "string")
				primitiveDatum.Set("", currentDatumSize);
			else if (elementName == "vector")
				primitiveDatum.Set(glm::vec4(0), currentDatumSize);
			else if (elementName == "matrix")
				primitiveDatum.Set(glm::mat4(0), currentDatumSize);

			// <integer name="variableName" value="variableValue"/>
			if (attributes.ContainsKey("value"))
			{
				if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::VALUE_START))
					throw std::exception("Invalid script syntax");
				if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::VALUE_END))
					throw std::exception("Invalid script syntax");

				primitiveDatum.SetFromString(attributes["value"], currentDatumSize);
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
			datum.SetFromString(mCharData, datum.Size() - 1);
		}

		// <integer>
		//		<name>varname</name>
		//		<value>10</value>
		// </integer>
		else if (sharedDataPtr->NameValueElementDataParsed)
		{
			Datum& primitiveDatum = sharedDataPtr->CurrentScopePtr->Append(sharedDataPtr->DataName);
			primitiveDatum.SetType(mElementMetaData[elementName].mType);
			primitiveDatum.SetFromString(sharedDataPtr->DataValue, primitiveDatum.Size());
		}

		if (!sharedDataPtr->CheckStateTransition(mElementMetaData[elementName].mEndState))
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
