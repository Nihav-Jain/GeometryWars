#pragma once
#include "IXmlParseHelper.h"
#include "SharedDataTable.h"

namespace Library
{
	/**
	 *	Helper to parse Action from the XML
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
		 *	disallow copy construction of XmlParseHelperAction
		 */
		XmlParseHelperAction(const XmlParseHelperAction& rhs) = delete;

		/**
		 *	disallow copy assignment of XmlParseHelperAction
		 */
		XmlParseHelperAction& operator=(const XmlParseHelperAction& rhs) = delete;

		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes) override;
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName) override;
		virtual IXmlParseHelper* Clone() const override;

	protected:
		/**
		 *	Called from the StartElementHandler, can be used to initialize any member variables of Action from the xml attributes
		 *	@param reference to the created Action
		 *	@param constant reference to the map of attribtues
		 */
		virtual void ParseActionAttributes(Action& currentAction, const Hashmap<std::string, std::string>& attributes);
		virtual void PostXMLParsing(Action& currentAction);

		static const std::string ATTRIBUTE_CLASS;
		static const std::string ATTRIBUTE_NAME;

		/** name of the action type element to be parsed */
		std::string mElementName;
		/** list of compulsory attributes for this action tag */
		Vector<std::string> mRequiredAttributes;
		/** name of the class of this Action tag */
		std::string mDerivedActionClassName;
		/** name of the instance of this Action */
		std::string mActionInstanceName;
		/** name of the instance of this Action */
		std::string mParentAttributeName;
	};
}

