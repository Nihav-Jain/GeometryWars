#include "pch.h"
#include "XmlParseHelperVector.h"

namespace Library
{
	XmlParseHelperVector::XmlParseHelperVector()
	{
	}


	XmlParseHelperVector::~XmlParseHelperVector()
	{
	}

	bool XmlParseHelperVector::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes)
	{
		XmlParseHelperTable::SharedDataTable* sharedDataPtr = sharedData.As<XmlParseHelperTable::SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (elementName != "vector")
			return false;
		if (!attributes.ContainsKey("name"))
			throw std::exception("Invalid script syntax. missing variable name.");

		if (!sharedDataPtr->CheckStateTransition(XmlParseHelperTable::SharedDataTable::ParserState::VECTOR_START))
			throw std::exception("Invalid script syntax");

		sharedDataPtr->ParsedElements.Push(elementName);
		Datum& vectorDatum = sharedDataPtr->ScopeStack.Top()->Append(attributes["name"]);
		vectorDatum = glm::vec4(0, 0, 0, 0);

		std::string vectorAttributeNames[] = { "x", "y", "z", "w" };
		std::uint32_t i;
		for (i = 0; i < 4; i++)
		{
			if (attributes.ContainsKey(vectorAttributeNames[i]))
			{
				vectorDatum.Get<glm::vec4>().data[i] = std::stof(attributes[vectorAttributeNames[i]]);
			}
		}

		return true;
	}

	bool XmlParseHelperVector::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName)
	{
		XmlParseHelperTable::SharedDataTable* sharedDataPtr = sharedData.As<XmlParseHelperTable::SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (elementName != "vector")
			return false;

		if (!sharedDataPtr->CheckStateTransition(XmlParseHelperTable::SharedDataTable::ParserState::VECTOR_END))
			throw std::exception("Invalid script syntax");
		if (!sharedDataPtr->CheckStateTransition(XmlParseHelperTable::SharedDataTable::ParserState::END_STATE_ROUTER))
			throw std::exception("Invalid script syntax");

		sharedDataPtr->ParsedElements.Pop();
		return true;
	}

	IXmlParseHelper* XmlParseHelperVector::Clone() const
	{
		return new XmlParseHelperVector();
	}
}
