#pragma once
#include "XmlParseHelperAction.h"
#include "SharedDataTable.h"

namespace Library
{
	/**
	*	Helper to parse Entity from the XML
	*/
	class XmlParseHelperAnimationState : public XmlParseHelperAction
	{
	public:
		/**
		*	Default constructor
		*/
		XmlParseHelperAnimationState();

		/**
		*	Default destructructor
		*/
		virtual ~XmlParseHelperAnimationState() = default;

		/**
		*	disallow copy construction of XmlParseHelperEntity
		*/
		XmlParseHelperAnimationState(const XmlParseHelperAnimationState& rhs) = delete;

		/**
		*	disallow copy assignment of XmlParseHelperEntity
		*/
		XmlParseHelperAnimationState& operator=(const XmlParseHelperAnimationState& rhs) = delete;

		virtual IXmlParseHelper* Clone() const override;
	};
}

