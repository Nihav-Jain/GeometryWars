#pragma once
#include "XmlParseHelperAction.h"
#include "SharedDataTable.h"

namespace Library
{
	/**
	*	Helper to parse Entity from the XML
	*/
	class XmlParseHelperCircleColliderComponent : public XmlParseHelperAction
	{
	public:
		/**
		*	Default constructor
		*/
		XmlParseHelperCircleColliderComponent();

		/**
		*	Default destructructor
		*/
		virtual ~XmlParseHelperCircleColliderComponent() = default;

		/**
		*	disallow copy construction of XmlParseHelperCircleColliderComponent
		*/
		XmlParseHelperCircleColliderComponent(const XmlParseHelperCircleColliderComponent& rhs) = delete;

		/**
		*	disallow copy assignment of XmlParseHelperCircleColliderComponent
		*/
		XmlParseHelperCircleColliderComponent& operator=(const XmlParseHelperCircleColliderComponent& rhs) = delete;

		virtual IXmlParseHelper* Clone() const override;

	protected:
		virtual void ParseActionAttributes(Action& currentAction, const Hashmap<std::string, std::string>& attributes) override;
	};
}