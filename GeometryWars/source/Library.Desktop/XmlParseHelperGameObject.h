#pragma once
#include "IXmlParseHelper.h"
#include "SharedDataTable.h"

namespace Library
{
	/**
	*	Helper to parse GameObject from the XML
	*/
	class XmlParseHelperGameObject : public IXmlParseHelper
	{
	public:
		/**
		*	Default constructor
		*/
		XmlParseHelperGameObject() = default;

		/**
		*	Default destructructor
		*/
		virtual ~XmlParseHelperGameObject() = default;

		/**
		*	disallow copy construction of XmlParseHelperGameObject
		*/
		XmlParseHelperGameObject(const XmlParseHelperGameObject& rhs) = delete;

		/**
		*	disallow copy assignment of XmlParseHelperGameObject
		*/
		XmlParseHelperGameObject& operator=(const XmlParseHelperGameObject& rhs) = delete;

		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes) override;
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName) override;
		virtual IXmlParseHelper* Clone() const override;

	private:
		const std::string ELEMENT_NAME = "gameobject";
		const std::string ATTRIBUTE_CLASS = "class";
		const std::string ATTRIBUTE_NAME = "name";
		const std::string ATTRIBUTE_POSITION = "position";
		const std::string ATTRIBUTE_ROTATION = "rotation";
		const std::string ATTRIBUTE_SCALE = "scale";
		const std::string ATTRIBUTE_MOVESPEED = "speed";
	};
}
