#pragma once
#include "IXmlParseHelper.h"
#include "XmlParseHelperTable.h"
#include "SharedDataTable.h"

namespace Library
{
	/**
	 *	Parser to handle <vector> element and convert it into a glm::vec4
	 */
	class XmlParseHelperVector : public IXmlParseHelper
	{
	public:
		XmlParseHelperVector() = default;
		virtual ~XmlParseHelperVector() = default;
		XmlParseHelperVector(const XmlParseHelperVector& rhs) = delete;

		XmlParseHelperVector& operator=(const XmlParseHelperVector& rhs) = delete;

		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes) override;
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName) override;
		virtual IXmlParseHelper* Clone() const override;
	};

}

