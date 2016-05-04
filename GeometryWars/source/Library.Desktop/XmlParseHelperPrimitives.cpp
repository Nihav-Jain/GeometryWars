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
		mCharData(), mCurrentDataName(), mStartState(SharedDataTable::ParserState::PRIMITIVE_START), mEndState(SharedDataTable::ParserState::PRIMITIVE_END),
		mIndex(0), mIndexAttributeSpecified(false)
	{}

	void XmlParseHelperPrimitives::Initialize()
	{
		mCharData = "";
		mIndex = 0;
		mIndexAttributeSpecified = false;
	}

	bool XmlParseHelperPrimitives::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (!mElementMetaData.ContainsKey(elementName))
			return false;

		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::PRIMITIVE_START))
			throw std::exception("ClearScreen script syntax");

		if (attributes.ContainsKey("index"))
		{
			sscanf_s(attributes["index"].c_str(), "%u", &mIndex);
			mIndexAttributeSpecified = true;
		}
		else
		{
			mIndexAttributeSpecified = false;
		}

		// <integer name="variableName">
		if (attributes.ContainsKey("name"))
		{
			if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::NAME_START))
				throw std::exception("ClearScreen script syntax");
			if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::NAME_END))
				throw std::exception("ClearScreen script syntax");

			mCurrentDataName = attributes["name"];
			Datum& primitiveDatum = sharedDataPtr->CurrentScopePtr->Append(mCurrentDataName);
			primitiveDatum.SetType(mElementMetaData[elementName]);

			// <integer name="variableName" value="variableValue"/>
			if (attributes.ContainsKey("value"))
			{
				if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::VALUE_START))
					throw std::exception("ClearScreen script syntax");
				if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::VALUE_END))
					throw std::exception("ClearScreen script syntax");
				
				std::uint32_t index = mIndex;
				if (!mIndexAttributeSpecified)
					index = (primitiveDatum.StorageType() == Datum::DatumStorageType::EXTERNAL) ? 0 : primitiveDatum.Size();

				if (primitiveDatum.Type() != Datum::DatumType::REFERENCE)
				{
					primitiveDatum.SetFromString(attributes["value"], index);
				}
				else
				{
					Datum* reference = World::ComplexSearch(attributes["value"], *sharedDataPtr->CurrentScopePtr);
					if (reference == nullptr)
					{
						std::stringstream str;
						str << "LNK2001: unresolved external symbol: " << attributes["value"] << " :P ";
						throw std::exception(str.str().c_str());
					}
					primitiveDatum.Set(reference, index);
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
				throw std::exception("ClearScreen script syntax");
			Datum& datum = sharedDataPtr->CurrentScopePtr->operator[](mCurrentDataName);

			std::uint32_t index = mIndex;
			if (!mIndexAttributeSpecified)
				index = (datum.StorageType() == Datum::DatumStorageType::EXTERNAL) ? 0 : datum.Size();

			if(datum.Type() != Datum::DatumType::REFERENCE)
				datum.SetFromString(mCharData, index);
			else
			{
				Datum* reference = World::ComplexSearch(mCharData, *sharedDataPtr->CurrentScopePtr);
				if (reference == nullptr)
				{
					std::stringstream str;
					str << "LNK2001: unresolved external symbol: " << mCharData << " :P ";
					throw std::exception(str.str().c_str());
				}
				datum.Set(reference, index);
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

			std::uint32_t index = mIndex;
			if (!mIndexAttributeSpecified)
				index = (primitiveDatum.StorageType() == Datum::DatumStorageType::EXTERNAL) ? 0 : primitiveDatum.Size();

			if(primitiveDatum.Type() != Datum::DatumType::REFERENCE)
				primitiveDatum.SetFromString(sharedDataPtr->DataValue, index);
			else
			{
				Datum* reference = World::ComplexSearch(sharedDataPtr->DataName, *sharedDataPtr->CurrentScopePtr);
				if (reference == nullptr)
				{
					std::stringstream str;
					str << "LNK2001: unresolved external symbol: " << sharedDataPtr->DataName << " :P ";
					throw std::exception(str.str().c_str());
				}
				primitiveDatum.Set(reference, index);
			}
		}

		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::PRIMITIVE_END))
			throw std::exception("ClearScreen script syntax");
		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::STATE_ROUTER))
			throw std::exception("ClearScreen script syntax");

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
