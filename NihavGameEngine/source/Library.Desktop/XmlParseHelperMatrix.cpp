#include "pch.h"
#include "XmlParseHelperMatrix.h"

namespace Library
{
	XmlParseHelperMatrix::XmlParseHelperMatrix()
	{
	}


	XmlParseHelperMatrix::~XmlParseHelperMatrix()
	{
	}

	bool XmlParseHelperMatrix::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes)
	{
		XmlParseHelperTable::SharedDataTable* sharedDataPtr = sharedData.As<XmlParseHelperTable::SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (elementName != "matrix")
			return false;
		if (!attributes.ContainsKey("name"))
			throw std::exception("Invalid script syntax. missing variable name.");

		if (!sharedDataPtr->CheckStateTransition(XmlParseHelperTable::SharedDataTable::ParserState::MATRIX_START))
			throw std::exception("Invalid script syntax");

		sharedDataPtr->ParsedElements.Push(elementName);
		Datum& matDatum = sharedDataPtr->ScopeStack.Top()->Append(attributes["name"]);
		matDatum = glm::mat4(0);

		std::string matAttributeNames[4][4] = { 
			{"x0", "x1", "x2", "x3"},
			{ "y0", "y1", "y2", "y3" },
			{ "z0", "z1", "z2", "z3" },
			{ "w0", "w1", "w2", "w3" }
		};
		std::uint32_t i;
		std::uint32_t j;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (attributes.ContainsKey(matAttributeNames[i][j]))
				{
					matDatum.Get<glm::mat4>()[i].data[j] = std::stof(attributes[matAttributeNames[i][j]]);
				}
			}
		}

		return true;
	}

	bool XmlParseHelperMatrix::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName)
	{
		XmlParseHelperTable::SharedDataTable* sharedDataPtr = sharedData.As<XmlParseHelperTable::SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (elementName != "matrix")
			return false;

		if (!sharedDataPtr->CheckStateTransition(XmlParseHelperTable::SharedDataTable::ParserState::MATRIX_END))
			throw std::exception("Invalid script syntax");
		if (!sharedDataPtr->CheckStateTransition(XmlParseHelperTable::SharedDataTable::ParserState::END_STATE_ROUTER))
			throw std::exception("Invalid script syntax");

		sharedDataPtr->ParsedElements.Pop();
		return true;
	}

	IXmlParseHelper* XmlParseHelperMatrix::Clone() const
	{
		return new XmlParseHelperMatrix();
	}
}
