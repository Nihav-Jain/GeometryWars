#include "pch.h"
#include "XmlParseHelperTable.h"

namespace Library
{
	Graph<XmlParseHelperTable::SharedDataTable::ParserState> XmlParseHelperTable::SharedDataTable::ParserStateAutomata;

	XmlParseHelperTable::XmlParseHelperTable()
	{

	}


	XmlParseHelperTable::~XmlParseHelperTable()
	{
	}

	void XmlParseHelperTable::Initialize()
	{

	}

	bool XmlParseHelperTable::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;

		if (elementName != "scope")
			return false;

		if (!attributes.ContainsKey("name"))
			throw std::exception("Invalid syntax for <scope>. Missing attribute: name");

		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::SCOPE_START, true))
			throw std::exception("Invalid script syntax");

		sharedDataPtr->ScopeStack.Push(&(sharedDataPtr->ScopeStack.Top()->AppendScope(attributes["name"])));

		return true;
	}

	bool XmlParseHelperTable::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName)
	{
		SharedDataTable* sharedDataPtr = sharedData.As<SharedDataTable>();
		if (sharedDataPtr == nullptr)
			return false;
		if (elementName != "scope")
			return false;

		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::SCOPE_END, true))
			throw std::exception("Invalid script syntax");

		sharedDataPtr->ScopeStack.Pop();

		return true;
	}

	IXmlParseHelper* XmlParseHelperTable::Clone() const
	{
		return new XmlParseHelperTable();
	}


#pragma region SharedDataTable

	RTTI_DEFINITIONS(XmlParseHelperTable::SharedDataTable);

	XmlParseHelperTable::SharedDataTable::SharedDataTable() :
		RootScope(), ScopeStack(), ParsedElements()
	{
		ScopeStack.Push(&RootScope);

		// prepare the state diagram graph here
		if (ParserStateAutomata.IsEmpty())
		{
			ParserState root = ParserState::ROOT_STATE;
			ParserState scopeStart = ParserState::SCOPE_START;
			ParserState scopeEnd = ParserState::SCOPE_END;
			//ParserState intStart = ParserState::INTEGER_START;
			//ParserState intEnd = ParserState::INTEGER_END;
			//ParserState nameStart = ParserState::NAME_START;
			//ParserState nameEnd = ParserState::NAME_END;
			//ParserState valueStart = ParserState::VALUE_START;
			//ParserState valueEnd = ParserState::VALUE_END;

			Graph<ParserState>::Traversor rootState = ParserStateAutomata.AddVertex(root);
			Graph<ParserState>::Traversor scopeStartState = ParserStateAutomata.AddVertex(scopeStart, rootState);
			Graph<ParserState>::Traversor scopeEndState = ParserStateAutomata.AddVertex(scopeEnd, scopeStartState);
			ParserStateAutomata.AddVertex(scopeStart, scopeEndState);
		}

		StateTraversor = ParserStateAutomata.Begin();
	}

	XmlParseHelperTable::SharedDataTable::~SharedDataTable()
	{
	}

	XmlParseMaster::SharedData* XmlParseHelperTable::SharedDataTable::Clone() const
	{
		return new SharedDataTable();
	}

	void XmlParseHelperTable::SharedDataTable::ClearStateGraph()
	{
		ParserStateAutomata.Clear();
	}

	bool XmlParseHelperTable::SharedDataTable::CheckStateTransition(ParserState expectedState, bool selfTransitionAllowed)
	{
		bool hasValidStateTransition = false;

		if (selfTransitionAllowed)
		{
			if (*StateTraversor == expectedState)
			{
				return true;
			}
		}
		while (StateTraversor.HasMoreChildren())
		{
			if (StateTraversor.GetCurrentChildVertex() == expectedState)
			{
				hasValidStateTransition = true;
				StateTraversor.TraverseToCurrentChild();
				break;
			}
			StateTraversor.MoveToNextChild();
		}
		return hasValidStateTransition;
	}

#pragma endregion

}
