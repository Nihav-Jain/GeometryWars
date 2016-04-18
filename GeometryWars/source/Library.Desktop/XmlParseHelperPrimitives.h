#pragma once
#include "IXmlParseHelper.h"
#include "XmlParseHelperTable.h"

#include "Vector.h"
#include "Hashmap.h"
#include "Datum.h"
#include "Scope.h"

namespace Library
{
	/**
	 *	Helper to parse Integers, Floats and Strings from the XML and create Datums in the current Scope
	 */
	class XmlParseHelperPrimitives : public IXmlParseHelper
	{
	public:
		
		/**
		 *	Parameterless constructor
		 */
		XmlParseHelperPrimitives();

		/**
		 *	default destructor
		 */
		virtual ~XmlParseHelperPrimitives() = default;

		/**
		 *	disallow copy constructor
		 */
		XmlParseHelperPrimitives(const XmlParseHelperPrimitives& rhs) = delete;

		/**
		 *	disallow copy assignment operator
		 */
		XmlParseHelperPrimitives& operator=(const XmlParseHelperPrimitives& rhs) = delete;

		virtual void Initialize() override;
		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes) override;
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName) override;
		virtual bool CharDataHandler(XmlParseMaster::SharedData& sharedData, const std::string& charData) override;
		virtual IXmlParseHelper* Clone() const override;

	private:

		struct MetaData
		{
			SharedDataTable::ParserState mStartState;
			SharedDataTable::ParserState mEndState;
			Datum::DatumType mType;
		};

		static Hashmap<std::string, Datum::DatumType> mElementMetaData;

		std::string mCharData;
		std::string mCurrentDataName;

		SharedDataTable::ParserState mStartState;
		SharedDataTable::ParserState mEndState;

		std::uint32_t mIndex;
		bool mIndexAttributeSpecified;
	};
}
