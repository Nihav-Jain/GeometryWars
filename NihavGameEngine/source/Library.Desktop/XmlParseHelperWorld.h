#pragma once
#include "IXmlParseHelper.h"
#include "SharedDataTable.h"

namespace Library
{
	class XmlParseHelperWorld : public IXmlParseHelper
	{
	public:
		/**
		 *	Default constructor
		 */
		XmlParseHelperWorld() = default;

		/**
		 *	Default destructructor
		 */
		virtual ~XmlParseHelperWorld() = default;

		/**
		 *	disallow copy construction of XmlParseHelperTable
		 */
		XmlParseHelperWorld(const XmlParseHelperWorld& rhs) = delete;

		/**
		 *	disallow copy assignment of XmlParseHelperTable
		 */
		XmlParseHelperWorld& operator=(const XmlParseHelperWorld& rhs) = delete;

		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes) override;
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName) override;
		virtual IXmlParseHelper* Clone() const override;

	private:
		const std::string ELEMENT_NAME = "world";
		const std::string ATTRIBUTE_NAME = "name";

	};

}

