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

		static void ClearStaticMembers();
	private:

		template <SharedDataTable::ParserState T>
		void ConvertValue(SharedDataTable& sharedData, const std::string& name, const std::string& valueStr);
		template<>
		void ConvertValue<SharedDataTable::ParserState::INTEGER_END>(SharedDataTable& sharedData, const std::string& name, const std::string& valueStr);
		template<>
		void ConvertValue<SharedDataTable::ParserState::FLOAT_END>(SharedDataTable& sharedData, const std::string& name, const std::string& valueStr);
		template<>
		void ConvertValue<SharedDataTable::ParserState::STRING_END>(SharedDataTable& sharedData, const std::string& name, const std::string& valueStr);

		typedef void (XmlParseHelperPrimitives::*ValueConvertor)(SharedDataTable&, const std::string&, const std::string&);
		Hashmap<std::string, ValueConvertor> mValueConvertors;
		Hashmap<std::string, SharedDataTable::ParserState> mElementParseEndStates;
		Hashmap<std::string, SharedDataTable::ParserState> mElementParseStartStates;

		std::string mCharData;
		std::string mCurrentDataName;
	};
}
