#pragma once
#include "XmlParseHelperAction.h"
#include "SharedDataTable.h"

namespace Library
{
	/**
	 *	Helper to parse Entity from the XML
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
		 *	disallow copy construction of XmlParseHelperEntity
		 */
		XmlParseHelperActionSwitch(const XmlParseHelperActionSwitch& rhs) = delete;

		/**
		 *	disallow copy assignment of XmlParseHelperEntity
		 */
		XmlParseHelperActionSwitch& operator=(const XmlParseHelperActionSwitch& rhs) = delete;

		virtual IXmlParseHelper* Clone() const override;

	public:
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
			*	disallow copy construction of XmlParseHelperEntity
			*/
			XmlParseHelperActionSwitchCase(const XmlParseHelperActionSwitchCase& rhs) = delete;

			/**
			*	disallow copy assignment of XmlParseHelperEntity
			*/
			XmlParseHelperActionSwitchCase& operator=(const XmlParseHelperActionSwitchCase& rhs) = delete;

			virtual IXmlParseHelper* Clone() const override;
		};
	};
}

