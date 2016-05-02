#pragma once
#include "IXmlParseHelper.h"
#include "SharedDataTable.h"

namespace Library
{
	/**
	 *	Helper to parse Entity from the XML
	 */
	class XmlParseHelperDefine : public IXmlParseHelper
	{
	public:
		/**
		 *	Default constructor
		 */
		XmlParseHelperDefine() = default;

		/**
		 *	Default destructructor
		 */
		virtual ~XmlParseHelperDefine() = default;

		/**
		 *	disallow copy construction of XmlParseHelperDefine
		 */
		XmlParseHelperDefine(const XmlParseHelperDefine& rhs) = delete;

		/**
		 *	disallow copy assignment of XmlParseHelperDefine
		 */
		XmlParseHelperDefine& operator=(const XmlParseHelperDefine& rhs) = delete;

		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes) override;
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName) override;
		virtual IXmlParseHelper* Clone() const override;

	private:
		Scope* mOriginalParent;
		const std::string ELEMENT_NAME = "define";
	};
}

