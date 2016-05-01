#pragma once
#include "IXmlParseHelper.h"
#include "SharedDataTable.h"

namespace Library {
	class XmlParseHelperSprite : public IXmlParseHelper
	{
	public:
		XmlParseHelperSprite();
		virtual ~XmlParseHelperSprite() = default;

		XmlParseHelperSprite(const XmlParseHelperSprite& rhs) = delete;
		XmlParseHelperSprite& operator=(const XmlParseHelperSprite& rhs) = delete;

		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes) override;
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName) override;
		virtual IXmlParseHelper* Clone() const override;

	private:
		static const std::string ELEMENT_NAME;
		static const std::string ATTRIBUTE_NAME;

	};
}
