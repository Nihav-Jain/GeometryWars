#pragma once
#include "XmlParseHelperAction.h"
#include "SharedDataTable.h"

namespace Library
{
	/**
	*	Helper to parse Entity from the XML
	*/
	class XmlParseHelperAnimator : public XmlParseHelperAction
	{
	public:
		/**
		*	Default constructor
		*/
		XmlParseHelperAnimator();

		/**
		*	Default destructructor
		*/
		virtual ~XmlParseHelperAnimator() = default;

		/**
		*	disallow copy construction of XmlParseHelperEntity
		*/
		XmlParseHelperAnimator(const XmlParseHelperAnimator& rhs) = delete;

		/**
		*	disallow copy assignment of XmlParseHelperEntity
		*/
		XmlParseHelperAnimator& operator=(const XmlParseHelperAnimator& rhs) = delete;

		virtual IXmlParseHelper* Clone() const override;
	};
}

