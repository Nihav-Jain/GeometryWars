#pragma once
#include "IXmlParseHelper.h"
#include "XmlParseMaster.h"
#include "SharedDataTable.h"

#include "Datum.h"
#include "Scope.h"
#include "Stack.h"
#include "Hashmap.h"
#include "Graph.h"

namespace Library
{
	/**
	 *	Helper to parse Scope tables from the XML
	 */
	class XmlParseHelperTable : public IXmlParseHelper
	{
	public:
		/**
		 *	Default constructor
		 */
		XmlParseHelperTable() = default;

		/**
		 *	Default destructructor
		 */
		virtual ~XmlParseHelperTable() = default;

		/**
		 *	disallow copy construction of XmlParseHelperTable
		 */
		XmlParseHelperTable(const XmlParseHelperTable& rhs) = delete;

		/**
		 *	disallow copy assignment of XmlParseHelperTable
		 */
		XmlParseHelperTable& operator=(const XmlParseHelperTable& rhs) = delete;

		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes) override;
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName) override;
		virtual IXmlParseHelper* Clone() const override;

	private:
		const std::string ELEMENT_SCOPE = "scope";
		const std::string ATTRIBUTE_NAME = "name";
	};
}

