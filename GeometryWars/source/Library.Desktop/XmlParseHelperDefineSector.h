#pragma once
#include "IXmlParseHelper.h"
#include "SharedDataTable.h"

namespace Library
{
	/**
	 *	Helper to parse Entity from the XML
	 */
	class XmlParseHelperDefineSector : public IXmlParseHelper
	{
	public:
		/**
		 *	Default constructor
		 */
		XmlParseHelperDefineSector() = default;

		/**
		 *	Default destructructor
		 */
		virtual ~XmlParseHelperDefineSector() = default;

		/**
		 *	disallow copy construction of XmlParseHelperDefineSector
		 */
		XmlParseHelperDefineSector(const XmlParseHelperDefineSector& rhs) = delete;

		/**
		 *	disallow copy assignment of XmlParseHelperDefineSector
		 */
		XmlParseHelperDefineSector& operator=(const XmlParseHelperDefineSector& rhs) = delete;

		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes) override;
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName) override;
		virtual IXmlParseHelper* Clone() const override;

	private:
		Scope* mOriginalParent;
		const std::string ELEMENT_NAME = "define-sectors";
	};
}

