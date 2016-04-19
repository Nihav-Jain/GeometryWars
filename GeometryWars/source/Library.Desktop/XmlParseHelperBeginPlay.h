#pragma once
#include "IXmlParseHelper.h"
#include "SharedDataTable.h"

namespace Library
{
	/**
	 *	Helper to parse Action from the XML
	 */
	class XmlParseHelperBeginPlay : public IXmlParseHelper
	{
	public:
		/**
		 *	Default constructor
		 */
		XmlParseHelperBeginPlay();

		/**
		 *	Default destructructor
		 */
		virtual ~XmlParseHelperBeginPlay() = default;

		/**
		 *	disallow copy construction of XmlParseHelperBeginPlay
		 */
		XmlParseHelperBeginPlay(const XmlParseHelperBeginPlay& rhs) = delete;

		/**
		 *	disallow copy assignment of XmlParseHelperBeginPlay
		 */
		XmlParseHelperBeginPlay& operator=(const XmlParseHelperBeginPlay& rhs) = delete;

		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes) override;
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName) override;
		virtual IXmlParseHelper* Clone() const override;

	private:
		static const std::string ELEMENT_NAME;
		static const std::string ATTRIBUTE_CLASS;
		static const std::string ATTRIBUTE_NAME;
	};
}

