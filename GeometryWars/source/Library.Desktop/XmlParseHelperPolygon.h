#pragma once
#include "IXmlParseHelper.h"
#include "SharedDataTable.h"

namespace Library {
	class XmlParseHelperPolygon : public IXmlParseHelper
	{
	public:
		XmlParseHelperPolygon();
		virtual ~XmlParseHelperPolygon() = default;

		XmlParseHelperPolygon(const XmlParseHelperPolygon& rhs) = delete;
		XmlParseHelperPolygon& operator=(const XmlParseHelperPolygon& rhs) = delete;

		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes) override;
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName) override;
		virtual IXmlParseHelper* Clone() const override;

	private:
		static const std::string ELEMENT_NAME;
		static const std::string ATTRIBUTE_NAME;
	};
}
