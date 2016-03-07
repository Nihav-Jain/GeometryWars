#pragma once

#include "Hashmap.h"
#include "XmlParseMaster.h"

namespace Library
{
	class IXmlParseHelper
	{
	public:
		IXmlParseHelper() = default;
		IXmlParseHelper(const IXmlParseHelper& rhs) = delete;
		virtual ~IXmlParseHelper() = default;
		IXmlParseHelper& operator=(const IXmlParseHelper& rhs) = delete;

		virtual void Initialize() = 0;
		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes) = 0;
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName) = 0;
		virtual bool CharDataHandler(XmlParseMaster::SharedData& sharedData, const std::string& charData, std::int32_t length) 
		{ 
			UNREFERENCED_PARAMETER(sharedData);
			UNREFERENCED_PARAMETER(charData);
			UNREFERENCED_PARAMETER(length);
			return false; 
		};
		virtual IXmlParseHelper* Clone() = 0;
	};
}

