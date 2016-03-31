#pragma once
#include "XmlParseHelperAction.h"
#include "SharedDataTable.h"

namespace Library
{
	/**
	 *	Helper to parse Switch from the XML
	 */
	class XmlParseHelperActionCreate : public XmlParseHelperAction
	{
	public:
		/**
		 *	Default constructor
		 */
		XmlParseHelperActionCreate();

		/**
		 *	Default destructructor
		 */
		virtual ~XmlParseHelperActionCreate() = default;

		/**
		 *	disallow copy construction of XmlParseHelperActionCreate
		 */
		XmlParseHelperActionCreate(const XmlParseHelperActionCreate& rhs) = delete;

		/**
		 *	disallow copy assignment of XmlParseHelperActionCreate
		 */
		XmlParseHelperActionCreate& operator=(const XmlParseHelperActionCreate& rhs) = delete;

		virtual IXmlParseHelper* Clone() const override;

	protected:
		/**
		 *	@override initilizes the prescribed attribtues
		 *	@param reference to the created Action
		 *	@param constant reference to the map of attribtues
		 */
		virtual void ParseActionAttributes(Action& action, const Hashmap<std::string, std::string>& attributes) override;
	};
}

