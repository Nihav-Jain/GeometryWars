#pragma once
#include "IXmlParseHelper.h"
#include "SharedDataTable.h"

namespace Library {
	class XmlParseHelperImage : public IXmlParseHelper
	{
	public:
		XmlParseHelperImage();
		virtual ~XmlParseHelperImage() = default;

		XmlParseHelperImage(const XmlParseHelperImage& rhs) = delete;
		XmlParseHelperImage& operator=(const XmlParseHelperImage& rhs) = delete;

		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes) override;
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName) override;
		virtual IXmlParseHelper* Clone() const override;

	private:
		static const std::string ELEMENT_NAME;
	};
}
