#pragma once

#include <string>
#include "Vector.h"
#include "Stack.h"
#include "IXmlParseHelper.h"
#include "AngryBirdsSharedData.h"

class AngryBirdsXmlParseHelper : public Library::IXmlParseHelper
{
public:
	AngryBirdsXmlParseHelper();
	AngryBirdsXmlParseHelper(const AngryBirdsXmlParseHelper& rhs);
	AngryBirdsXmlParseHelper(AngryBirdsXmlParseHelper&& rhs);
	virtual ~AngryBirdsXmlParseHelper() = default;

	AngryBirdsXmlParseHelper& operator=(const AngryBirdsXmlParseHelper& rhs);
	AngryBirdsXmlParseHelper& operator=(AngryBirdsXmlParseHelper&& rhs);

	virtual void Initialize() override;
	virtual bool StartElementHandler(Library::XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Library::Hashmap<std::string, std::string>& attributes) override;
	virtual bool EndElementHandler(Library::XmlParseMaster::SharedData& sharedData, const std::string& elementName) override;
	virtual IXmlParseHelper* Clone() const override;

	static void ClearElementParsers();

private:
	enum class ElementNames
	{
		AngryBirds,
		Birds,
		Bird,
		Physics,
		Sprites,
		Sprite
	};

	template <ElementNames T>
	bool ParseElement(AngryBirdsSharedData& sharedData, const Library::Hashmap<std::string, std::string>& attributes);
	template<>
	bool ParseElement<ElementNames::AngryBirds>(AngryBirdsSharedData& sharedData, const Library::Hashmap<std::string, std::string>& attributes);
	template<>
	bool ParseElement<ElementNames::Birds>(AngryBirdsSharedData& sharedData, const Library::Hashmap<std::string, std::string>& attributes);

	typedef bool (AngryBirdsXmlParseHelper::*ElementParser)(AngryBirdsSharedData&, const Library::Hashmap<std::string, std::string>&);
	static Library::Hashmap<std::string, ElementParser> mElementParsers;

	Library::Stack<std::string> mElementStack;
};

