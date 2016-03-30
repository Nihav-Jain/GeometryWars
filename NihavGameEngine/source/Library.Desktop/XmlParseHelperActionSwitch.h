#pragma once
#include "XmlParseHelperAction.h"
#include "SharedDataTable.h"

namespace Library
{
	/**
	 *	Helper to parse Switch from the XML
	 */
	class XmlParseHelperActionSwitch : public XmlParseHelperAction
	{
	public:
		/**
		 *	Default constructor
		 */
		XmlParseHelperActionSwitch();

		/**
		 *	Default destructructor
		 */
		virtual ~XmlParseHelperActionSwitch() = default;

		/**
		 *	disallow copy construction of XmlParseHelperActionSwitch
		 */
		XmlParseHelperActionSwitch(const XmlParseHelperActionSwitch& rhs) = delete;

		/**
		 *	disallow copy assignment of XmlParseHelperActionSwitch
		 */
		XmlParseHelperActionSwitch& operator=(const XmlParseHelperActionSwitch& rhs) = delete;

		virtual IXmlParseHelper* Clone() const override;

	protected:
		/**
		 *	@override initializes the switchValue
		 *	@param reference to the created Action
		 *	@param constant reference to the map of attribtues
		 */
		virtual void ParseActionAttributes(Action& action, const Hashmap<std::string, std::string>& attributes) override;

	public:
		/**
		 *	Helper to parse Case from the XML
		 */
		class XmlParseHelperActionSwitchCase : public XmlParseHelperAction
		{
		public:
			/**
			*	Default constructor
			*/
			XmlParseHelperActionSwitchCase();

			/**
			*	Default destructructor
			*/
			virtual ~XmlParseHelperActionSwitchCase() = default;

			/**
			*	disallow copy construction of XmlParseHelperActionSwitchCase
			*/
			XmlParseHelperActionSwitchCase(const XmlParseHelperActionSwitchCase& rhs) = delete;

			/**
			*	disallow copy assignment of XmlParseHelperActionSwitchCase
			*/
			XmlParseHelperActionSwitchCase& operator=(const XmlParseHelperActionSwitchCase& rhs) = delete;

			virtual IXmlParseHelper* Clone() const override;

		protected:
			/**
			 *	@override initializes the caseValue, break and default attributes
			 *	@param reference to the created Action
			 *	@param constant reference to the map of attribtues
			 */
			virtual void ParseActionAttributes(Action& action, const Hashmap<std::string, std::string>& attributes) override;
		};
	};
}

