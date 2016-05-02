#pragma once
#include "XmlParseHelperAction.h"
#include "SharedDataTable.h"

namespace Library
{
	/**
	*	Helper to parse Entity from the XML
	*/
	class XmlParseHelperAnimationFrame : public XmlParseHelperAction
	{
	public:
		/**
		*	Default constructor
		*/
		XmlParseHelperAnimationFrame();

		/**
		*	Default destructructor
		*/
		virtual ~XmlParseHelperAnimationFrame() = default;

		/**
		*	disallow copy construction of XmlParseHelperEntity
		*/
		XmlParseHelperAnimationFrame(const XmlParseHelperAnimationFrame& rhs) = delete;

		/**
		*	disallow copy assignment of XmlParseHelperEntity
		*/
		XmlParseHelperAnimationFrame& operator=(const XmlParseHelperAnimationFrame& rhs) = delete;

		virtual IXmlParseHelper* Clone() const override;
	};
}

