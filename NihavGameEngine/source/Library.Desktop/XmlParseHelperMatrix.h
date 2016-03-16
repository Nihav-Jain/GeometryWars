#pragma once
#include "IXmlParseHelper.h"
#include "XmlParseHelperTable.h"

namespace Library
{
	class XmlParseHelperMatrix : public IXmlParseHelper
	{
	public:
		XmlParseHelperMatrix();
		virtual ~XmlParseHelperMatrix();
		XmlParseHelperMatrix(const XmlParseHelperMatrix& rhs) = delete;

		XmlParseHelperMatrix& operator=(const XmlParseHelperMatrix& rhs) = delete;

		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes) override;
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName) override;
		virtual IXmlParseHelper* Clone() const override;
	};

}

