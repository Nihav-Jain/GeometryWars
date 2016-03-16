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
		sharedDataPtr->ParsedElements.Push(elementName);

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
		if (!sharedDataPtr->CheckStateTransition(SharedDataTable::ParserState::END_STATE_ROUTER))
			throw std::exception("Invalid script syntax");

		sharedDataPtr->ScopeStack.Pop();
		sharedDataPtr->ParsedElements.Pop();

		return true;
	}

	IXmlParseHelper* XmlParseHelperTable::Clone() const
	{
		return new XmlParseHelperTable();
	}


#pragma region SharedDataTable

	RTTI_DEFINITIONS(XmlParseHelperTable::SharedDataTable);

	XmlParseHelperTable::SharedDataTable::SharedDataTable() :
		RootScope(), ScopeStack(), ParsedElements(), DataName(), DataValue(), NameValueElementDataParsed(false)
	{
		ScopeStack.Push(&RootScope);

		// prepare the state diagram graph here
		if (ParserStateAutomata.IsEmpty())
		{
			ParserState root = ParserState::ROOT_STATE;
			ParserState endRouter = ParserState::END_STATE_ROUTER;
			ParserState scopeStart = ParserState::SCOPE_START;
			ParserState scopeEnd = ParserState::SCOPE_END;
			ParserState intStart = ParserState::INTEGER_START;
			ParserState intEnd = ParserState::INTEGER_END;
			ParserState nameStart = ParserState::NAME_START;
			ParserState nameEnd = ParserState::NAME_END;
			ParserState valueStart = ParserState::VALUE_START;
			ParserState valueEnd = ParserState::VALUE_END;
			ParserState floatStart = ParserState::FLOAT_START;
			ParserState floatEnd = ParserState::FLOAT_END;
			ParserState stringStart = ParserState::STRING_START;
			ParserState stringEnd = ParserState::STRING_END;


			// root -> end_state_router
			// root -> scope_start -> scope_end -> end_state_router
			Graph<ParserState>::Traversor rootState = ParserStateAutomata.AddVertex(root);
			Graph<ParserState>::Traversor scopeStartState = ParserStateAutomata.AddVertex(scopeStart, rootState);
			Graph<ParserState>::Traversor endRouterState = ParserStateAutomata.AddVertex(endRouter, rootState);
			Graph<ParserState>::Traversor scopeEndState = ParserStateAutomata.AddVertex(scopeEnd, scopeStartState);
			ParserStateAutomata.CreateEdge(scopeEndState, endRouterState);

			// scope_start -> int_start -> name_start -> name_end -> value_start -> value_end -> int_end
			// int_end -> end_state_router
			Graph<ParserState>::Traversor intStartState = ParserStateAutomata.AddVertex(intStart, scopeStartState);
			Graph<ParserState>::Traversor nameStartState = ParserStateAutomata.AddVertex(nameStart, intStartState);
			Graph<ParserState>::Traversor nameEndState = ParserStateAutomata.AddVertex(nameEnd, nameStartState);
			Graph<ParserState>::Traversor valueStartState = ParserStateAutomata.AddVertex(valueStart, nameEndState);
			Graph<ParserState>::Traversor valueEndState = ParserStateAutomata.AddVertex(valueEnd, valueStartState);
			Graph<ParserState>::Traversor intEndState = ParserStateAutomata.AddVertex(intEnd, valueEndState);
			ParserStateAutomata.CreateEdge(intEndState, endRouterState);

			// scope_start -> float_start -> name_start -> name_end -> value_start -> value_end -> float_end
			// float_end -> end_state_router
			Graph<ParserState>::Traversor floatStartState = ParserStateAutomata.AddVertex(floatStart, scopeStartState);
			Graph<ParserState>::Traversor fnameStartState = ParserStateAutomata.AddVertex(nameStart, floatStartState);
			Graph<ParserState>::Traversor fnameEndState = ParserStateAutomata.AddVertex(nameEnd, fnameStartState);
			Graph<ParserState>::Traversor fvalueStartState = ParserStateAutomata.AddVertex(valueStart, fnameEndState);
			Graph<ParserState>::Traversor fvalueEndState = ParserStateAutomata.AddVertex(valueEnd, fvalueStartState);
			Graph<ParserState>::Traversor floatEndState = ParserStateAutomata.AddVertex(floatEnd, fvalueEndState);
			ParserStateAutomata.CreateEdge(floatEndState, endRouterState);

			// scope_start -> string_start -> name_start -> name_end -> value_start -> value_end -> string_end
			// string_end -> end_state_router
			Graph<ParserState>::Traversor stringStartState = ParserStateAutomata.AddVertex(stringStart, scopeStartState);
			Graph<ParserState>::Traversor snameStartState = ParserStateAutomata.AddVertex(nameStart, stringStartState);
			Graph<ParserState>::Traversor snameEndState = ParserStateAutomata.AddVertex(nameEnd, snameStartState);
			Graph<ParserState>::Traversor svalueStartState = ParserStateAutomata.AddVertex(valueStart, snameEndState);
			Graph<ParserState>::Traversor svalueEndState = ParserStateAutomata.AddVertex(valueEnd, svalueStartState);
			Graph<ParserState>::Traversor stringEndState = ParserStateAutomata.AddVertex(stringEnd, svalueEndState);
			ParserStateAutomata.CreateEdge(stringEndState, endRouterState);


			// connect end_state_router to all possible start states
			ParserStateAutomata.CreateEdge(endRouterState, scopeStartState);
			ParserStateAutomata.CreateEdge(endRouterState, intStartState);
			ParserStateAutomata.CreateEdge(endRouterState, floatStartState);
			ParserStateAutomata.CreateEdge(endRouterState, stringStartState);

			// also connect end_state_router to the scope_end state
			ParserStateAutomata.CreateEdge(endRouterState, scopeEndState);

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
		StateTraversor.ResetChildrenIterator();
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
