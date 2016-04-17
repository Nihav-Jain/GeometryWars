#include "pch.h"
#include "XmlParseHelperPrimitives.h"

namespace Library
{
	Hashmap<std::string, Datum::DatumType> XmlParseHelperPrimitives::mElementMetaData = {
		{ "integer", Datum::DatumType::INTEGER },
		{ "float", Datum::DatumType::FLOAT },
		{ "string", Datum::DatumType::STRING },
		{ "vector", Datum::DatumType::VECTOR4 },
		{ "matrix", Datum::DatumType::MATRIX4x4 },
		{ "bool", Datum::DatumType::BOOLEAN },
		{ "ref", Datum::DatumType::REFERENCE }
	};

	XmlParseHelperPrimitives::XmlParseHelperPrimitives() :
		mCharData(), mCurrentDataName(), mStartState(SharedDataTable::ParserState::PRIMITIVE_START), mEndState(SharedDataTable::ParserState::PRIMITIVE_END)
	{}

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

		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::PRIMITIVE_START))
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

				if (primitiveDatum.Type() != Datum::DatumType::REFERENCE)
				{
					if(primitiveDatum.StorageType() == Datum::DatumStorageType::EXTERNAL)
						primitiveDatum.SetFromString(attributes["value"], 0);
					else
						primitiveDatum.SetFromString(attributes["value"], primitiveDatum.Size());
				}
				else
				{
					Datum* reference = sharedDataPtr->CurrentScopePtr->Search(attributes["value"]);
					if (reference == nullptr)
					{
						std::stringstream str;
						str << "LNK2001: unresolved external symbol: " << attributes["value"] << " :P ";
						throw std::exception(str.str().c_str());
					}
					primitiveDatum = reference;
				}
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
			if(datum.Type() != Datum::DatumType::REFERENCE)
				datum.SetFromString(mCharData, datum.Size());
			else
			{
				Datum* reference = sharedDataPtr->CurrentScopePtr->Search(mCharData);
				if (reference == nullptr)
				{
					std::stringstream str;
					str << "LNK2001: unresolved external symbol: " << mCharData << " :P ";
					throw std::exception(str.str().c_str());
				}
				datum = reference;
			}
		}

		// <integer>
		//		<name>varname</name>
		//		<value>10</value>
		// </integer>
		else if (sharedDataPtr->NameValueElementDataParsed)
		{
			Datum& primitiveDatum = sharedDataPtr->CurrentScopePtr->Append(sharedDataPtr->DataName);
			primitiveDatum.SetType(mElementMetaData[elementName]);
			if(primitiveDatum.Type() != Datum::DatumType::REFERENCE)
				primitiveDatum.SetFromString(sharedDataPtr->DataValue, primitiveDatum.Size());
			else
			{
				Datum* reference = sharedDataPtr->CurrentScopePtr->Search(sharedDataPtr->DataName);
				if (reference == nullptr)
				{
					std::stringstream str;
					str << "LNK2001: unresolved external symbol: " << sharedDataPtr->DataName << " :P ";
					throw std::exception(str.str().c_str());
				}
				primitiveDatum = reference;
			}
		}

		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::PRIMITIVE_END))
			throw std::exception("Invalid script syntax");
		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER))
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

}
