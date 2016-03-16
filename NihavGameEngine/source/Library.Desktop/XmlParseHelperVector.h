#pragma once
#include "IXmlParseHelper.h"
#include "XmlParseHelperTable.h"

namespace Library
{
	class XmlParseHelperVector : public IXmlParseHelper
	{
	public:
		XmlParseHelperVector();
		virtual ~XmlParseHelperVector();
		XmlParseHelperVector(const XmlParseHelperVector& rhs) = delete;

		XmlParseHelperVector& operator=(const XmlParseHelperVector& rhs) = delete;

		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes) override;
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName) override;
		virtual IXmlParseHelper* Clone() const override;
	};

}

