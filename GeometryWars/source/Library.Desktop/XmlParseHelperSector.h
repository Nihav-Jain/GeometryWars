#pragma once
#include "IXmlParseHelper.h"
#include "SharedDataTable.h"

namespace Library
{
	/**
	 *	Helper to parse Sectors from the XML
	 */
	class XmlParseHelperSector : public IXmlParseHelper
	{
	public:
		/**
		*	Default constructor
		*/
		XmlParseHelperSector() = default;

		/**
		*	Default destructructor
		*/
		virtual ~XmlParseHelperSector() = default;

		/**
		*	disallow copy construction of XmlParseHelperSector
		*/
		XmlParseHelperSector(const XmlParseHelperSector& rhs) = delete;

		/**
		*	disallow copy assignment of XmlParseHelperSector
		*/
		XmlParseHelperSector& operator=(const XmlParseHelperSector& rhs) = delete;

		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes) override;
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName) override;
		virtual IXmlParseHelper* Clone() const override;

	private:
		const std::string ELEMENT_NAME = "sector";
		const std::string ATTRIBUTE_NAME = "name";
	};
}

