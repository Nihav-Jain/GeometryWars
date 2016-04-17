#pragma once
#include "XmlParseHelperAction.h"
#include "SharedDataTable.h"

namespace Library
{
	/**
	 *	Helper to parse Entity from the XML
	 */
	class XmlParseHelperActionWhile : public XmlParseHelperAction
	{
	public:
		/**
		 *	Default constructor
		 */
		XmlParseHelperActionWhile();

		/**
		 *	Default destructructor
		 */
		virtual ~XmlParseHelperActionWhile() = default;

		/**
		 *	disallow copy construction of XmlParseHelperEntity
		 */
		XmlParseHelperActionWhile(const XmlParseHelperActionWhile& rhs) = delete;

		/**
		 *	disallow copy assignment of XmlParseHelperEntity
		 */
		XmlParseHelperActionWhile& operator=(const XmlParseHelperActionWhile& rhs) = delete;

		virtual IXmlParseHelper* Clone() const override;
	
		class XmlParseHelperActionWhileLoopBody : public XmlParseHelperAction
		{
		public:
			/**
			*	Default constructor
			*/
			XmlParseHelperActionWhileLoopBody();

			/**
			*	Default destructructor
			*/
			virtual ~XmlParseHelperActionWhileLoopBody() = default;

			/**
			*	disallow copy construction of XmlParseHelperEntity
			*/
			XmlParseHelperActionWhileLoopBody(const XmlParseHelperActionWhileLoopBody& rhs) = delete;

			/**
			*	disallow copy assignment of XmlParseHelperEntity
			*/
			XmlParseHelperActionWhileLoopBody& operator=(const XmlParseHelperActionWhileLoopBody& rhs) = delete;

			virtual IXmlParseHelper* Clone() const override;
		};
	
	};
}

