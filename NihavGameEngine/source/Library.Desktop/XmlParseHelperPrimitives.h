#pragma once
#include "IXmlParseHelper.h"
#include "XmlParseHelperTable.h"

#include "Vector.h"
#include "Hashmap.h"
#include "Datum.h"
#include "Scope.h"

namespace Library
{
	class XmlParseHelperPrimitives : public IXmlParseHelper
	{
	public:
		XmlParseHelperPrimitives();
		XmlParseHelperPrimitives(const XmlParseHelperPrimitives& rhs) = delete;
		virtual ~XmlParseHelperPrimitives();

		XmlParseHelperPrimitives& operator=(const XmlParseHelperPrimitives& rhs) = delete;

		virtual void Initialize() override;
		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes) override;
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName) override;
		virtual bool CharDataHandler(XmlParseMaster::SharedData& sharedData, const std::string& charData) override;
		virtual IXmlParseHelper* Clone() const override;

	private:

		template <XmlParseHelperTable::SharedDataTable::ParserState T>
		void ConvertValue(XmlParseHelperTable::SharedDataTable& sharedData, const std::string& name, const std::string& valueStr);
		template<>
		void ConvertValue<XmlParseHelperTable::SharedDataTable::ParserState::INTEGER_END>(XmlParseHelperTable::SharedDataTable& sharedData, const std::string& name, const std::string& valueStr);
		template<>
		void ConvertValue<XmlParseHelperTable::SharedDataTable::ParserState::FLOAT_END>(XmlParseHelperTable::SharedDataTable& sharedData, const std::string& name, const std::string& valueStr);
		template<>
		void ConvertValue<XmlParseHelperTable::SharedDataTable::ParserState::STRING_END>(XmlParseHelperTable::SharedDataTable& sharedData, const std::string& name, const std::string& valueStr);

		typedef void (XmlParseHelperPrimitives::*ValueConvertor)(XmlParseHelperTable::SharedDataTable&, const std::string&, const std::string&);
		static Hashmap<std::string, ValueConvertor> mValueConvertors;
		static Hashmap<std::string, XmlParseHelperTable::SharedDataTable::ParserState> mElementParseEndStates;
		static Hashmap<std::string, XmlParseHelperTable::SharedDataTable::ParserState> mElementParseStartStates;

		std::string mCharData;
		std::string mCurrentDataName;
	};
}
