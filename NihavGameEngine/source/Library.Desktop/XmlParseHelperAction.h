#pragma once
#include "IXmlParseHelper.h"
#include "SharedDataTable.h"

namespace Library
{
	/**
	 *	Helper to parse Entity from the XML
	 */
	class XmlParseHelperAction : public IXmlParseHelper
	{
	public:
		/**
		 *	Default constructor
		 */
		XmlParseHelperAction();

		/**
		 *	Default destructructor
		 */
		virtual ~XmlParseHelperAction() = default;

		/**
		 *	disallow copy construction of XmlParseHelperEntity
		 */
		XmlParseHelperAction(const XmlParseHelperAction& rhs) = delete;

		/**
		 *	disallow copy assignment of XmlParseHelperEntity
		 */
		XmlParseHelperAction& operator=(const XmlParseHelperAction& rhs) = delete;

		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes) override;
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName) override;
		virtual IXmlParseHelper* Clone() const override;

	protected:
		virtual void PostActionParsing(Action& currentAction) const;

		static const std::string ATTRIBUTE_CLASS;
		static const std::string ATTRIBUTE_NAME;

		std::string mElementName;
		Vector<std::string> mRequiredAttributes;
		std::string mDerivedActionClassName;
		std::string mActionInstanceName;
	};
}

