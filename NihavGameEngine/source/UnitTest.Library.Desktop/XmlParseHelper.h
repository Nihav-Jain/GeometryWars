#pragma once

#include <string>
#include "Vector.h"
#include "Stack.h"
#include "IXmlParseHelper.h"
#include "XmlParseMaster.h"

class XmlParseHelper : public Library::IXmlParseHelper
{
public:
	XmlParseHelper() = default;
	virtual ~XmlParseHelper() = default;

	virtual void Initialize() override;
	virtual bool StartElementHandler(Library::XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Library::Hashmap<std::string, std::string>& attributes) override;
	virtual bool EndElementHandler(Library::XmlParseMaster::SharedData& sharedData, const std::string& elementName) override;
	virtual bool CharDataHandler(Library::XmlParseMaster::SharedData& sharedData, const std::string& charData) override;
	virtual IXmlParseHelper* Clone() const override;
};

