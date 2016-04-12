#pragma once
#include "XmlParseHelperAction.h"
#include "SharedDataTable.h"

namespace Library
{
	/**
	 *	Helper to parse Entity from the XML
	 */
	class XmlParseHelperActionExpression : public XmlParseHelperAction
	{
	public:
		/**
		 *	Default constructor
		 */
		XmlParseHelperActionExpression();

		/**
		 *	Default destructructor
		 */
		virtual ~XmlParseHelperActionExpression() = default;

		/**
		 *	disallow copy construction of XmlParseHelperEntity
		 */
		XmlParseHelperActionExpression(const XmlParseHelperActionExpression& rhs) = delete;

		/**
		 *	disallow copy assignment of XmlParseHelperEntity
		 */
		XmlParseHelperActionExpression& operator=(const XmlParseHelperActionExpression& rhs) = delete;

		virtual IXmlParseHelper* Clone() const override;
	};
}

