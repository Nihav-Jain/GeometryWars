#pragma once
#include "XmlParseHelperAction.h"
#include "SharedDataTable.h"

namespace Library
{
	/**
	 *	Helper to parse Entity from the XML
	 */
	class XmlParseHelperActionEvent: public XmlParseHelperAction
	{
	public:
		/**
		 *	Default constructor
		 */
		XmlParseHelperActionEvent();

		/**
		 *	Default destructructor
		 */
		virtual ~XmlParseHelperActionEvent() = default;

		/**
		 *	disallow copy construction of XmlParseHelperActionEvent
		 */
		XmlParseHelperActionEvent(const XmlParseHelperActionEvent& rhs) = delete;

		/**
		 *	disallow copy assignment of XmlParseHelperActionEvent
		 */
		XmlParseHelperActionEvent& operator=(const XmlParseHelperActionEvent& rhs) = delete;

		virtual IXmlParseHelper* Clone() const override;

	protected:
		virtual void ParseActionAttributes(Action& currentAction, const Hashmap<std::string, std::string>& attributes) override;
	};
}

