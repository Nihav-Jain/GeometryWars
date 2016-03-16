#pragma once

#include "Hashmap.h"
#include "XmlParseMaster.h"

namespace Library
{
	/**
	 *	Abstract class to serve as a template for specialized parse helpers
	 */
	class IXmlParseHelper
	{
	public:
		IXmlParseHelper() = default;
		virtual ~IXmlParseHelper() = default;
		IXmlParseHelper(const IXmlParseHelper& rhs) = delete;
		IXmlParseHelper& operator=(const IXmlParseHelper& rhs) = delete;

		/**
		 *	Reinitializes the member variables to be preapred for a fresh XML
		 */
		virtual void Initialize() {};

		/**
		 *	Handles the opening tag of XML elements
		 *	@param reference to the shared data
		 *	@param name of the element whose opening tag was encountered
		 *	@param hashmap of the attributes and their values
		 *	@return boolean to indicate if this version of the helper can handle the given element or not
		 */
		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes) = 0;

		/**
		 *	Handles the closing tag of XML elements
		 *	@param reference to the shared data
		 *	@param name of the element whose opening tag was encountered
		 *	@return boolean to indicate if this version of the helper can handle the given element or not
		 */
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName) = 0;

		/**
		 *	Handles the character data between an opening and closing tag
		 *	@param reference to the shared data
		 *	@param character data
		 *	@return boolean to indicate if this version of the helper can handle the given element or not
		 */
		virtual bool CharDataHandler(XmlParseMaster::SharedData& sharedData, const std::string& charData)
		{ 
			UNREFERENCED_PARAMETER(sharedData);
			UNREFERENCED_PARAMETER(charData);
			return false; 
		};

		/**
		 *	Creates a new XmlParseHelper
		 *	@return pointer to the newly created helper
		 */
		virtual IXmlParseHelper* Clone() const = 0;
	};
}

