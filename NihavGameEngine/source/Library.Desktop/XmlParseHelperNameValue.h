#pragma once
#include <string>

#include "IXmlParseHelper.h"
#include "XmlParseHelperTable.h"
#include "Vector.h"


namespace Library
{
	/**
	 *	Parser to handle <name> and <value> elements
	 */
	class XmlParseHelperNameValue : public IXmlParseHelper
	{
	public:
		XmlParseHelperNameValue();
		virtual ~XmlParseHelperNameValue() = default;

		XmlParseHelperNameValue(const XmlParseHelperNameValue& rhs) = delete;
		XmlParseHelperNameValue& operator=(const XmlParseHelperNameValue& rhs) = delete;

		virtual void Initialize();
		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes);
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName);
		virtual bool CharDataHandler(XmlParseMaster::SharedData& sharedData, const std::string& charData);
		virtual IXmlParseHelper* Clone() const;

	private:
		std::string mCharData;
	};
}

