#pragma once
#include "XmlParseHelperAction.h"
#include "SharedDataTable.h"

namespace Library
{
	/**
	 *	Helper to parse Entity from the XML
	 */
	class XmlParseHelperActionIf : public XmlParseHelperAction
	{
	public:
		/**
		 *	Default constructor
		 */
		XmlParseHelperActionIf();

		/**
		 *	Default destructructor
		 */
		virtual ~XmlParseHelperActionIf() = default;

		/**
		 *	disallow copy construction of XmlParseHelperEntity
		 */
		XmlParseHelperActionIf(const XmlParseHelperActionIf& rhs) = delete;

		/**
		 *	disallow copy assignment of XmlParseHelperEntity
		 */
		XmlParseHelperActionIf& operator=(const XmlParseHelperActionIf& rhs) = delete;

		virtual IXmlParseHelper* Clone() const override;

		class XmlParseHelperActionIfCondition : public XmlParseHelperAction
		{
		public:
			/**
			*	Default constructor
			*/
			XmlParseHelperActionIfCondition();

			/**
			*	Default destructructor
			*/
			virtual ~XmlParseHelperActionIfCondition() = default;

			/**
			*	disallow copy construction of XmlParseHelperEntity
			*/
			XmlParseHelperActionIfCondition(const XmlParseHelperActionIfCondition& rhs) = delete;

			/**
			*	disallow copy assignment of XmlParseHelperEntity
			*/
			XmlParseHelperActionIfCondition& operator=(const XmlParseHelperActionIfCondition& rhs) = delete;

			virtual IXmlParseHelper* Clone() const override;

		protected:
			virtual void PostXMLParsing(Action& currentAction) override;
		};
	
		class XmlParseHelperActionIfThen : public XmlParseHelperAction
		{
		public:
			/**
			*	Default constructor
			*/
			XmlParseHelperActionIfThen();

			/**
			*	Default destructructor
			*/
			virtual ~XmlParseHelperActionIfThen() = default;

			/**
			*	disallow copy construction of XmlParseHelperEntity
			*/
			XmlParseHelperActionIfThen(const XmlParseHelperActionIfThen& rhs) = delete;

			/**
			*	disallow copy assignment of XmlParseHelperEntity
			*/
			XmlParseHelperActionIfThen& operator=(const XmlParseHelperActionIfThen& rhs) = delete;

			virtual IXmlParseHelper* Clone() const override;
		};
	
		class XmlParseHelperActionIfElse : public XmlParseHelperAction
		{
		public:
			/**
			*	Default constructor
			*/
			XmlParseHelperActionIfElse();

			/**
			*	Default destructructor
			*/
			virtual ~XmlParseHelperActionIfElse() = default;

			/**
			*	disallow copy construction of XmlParseHelperEntity
			*/
			XmlParseHelperActionIfElse(const XmlParseHelperActionIfElse& rhs) = delete;

			/**
			*	disallow copy assignment of XmlParseHelperEntity
			*/
			XmlParseHelperActionIfElse& operator=(const XmlParseHelperActionIfElse& rhs) = delete;

			virtual IXmlParseHelper* Clone() const override;
		};
	};
}

