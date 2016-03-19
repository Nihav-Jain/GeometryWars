#include "pch.h"
#include "SharedDataTable.h"


namespace Library
{
	RTTI_DEFINITIONS(SharedDataTable);

	//Graph<SharedDataTable::ParserState> SharedDataTable::ParserStateAutomata;

	SharedDataTable::SharedDataTable() :
		mRootScope(), CurrentScopePtr(&mRootScope), ParsedElements(), DataName(), DataValue(), NameValueElementDataParsed(false), mStateTraversor()
	{

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
			ParserState vectorStart = ParserState::VECTOR_START;
			ParserState vectorEnd = ParserState::VECTOR_END;
			ParserState matStart = ParserState::MATRIX_START;
			ParserState matEnd = ParserState::MATRIX_END;


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

			// scope_start -> vector_start -> vector_end
			// vector_end -> end_state_router
			Graph<ParserState>::Traversor vectorStartState = ParserStateAutomata.AddVertex(vectorStart, scopeStartState);
			Graph<ParserState>::Traversor vnameStartState = ParserStateAutomata.AddVertex(nameStart, vectorStartState);
			Graph<ParserState>::Traversor vnameEndState = ParserStateAutomata.AddVertex(nameEnd, vnameStartState);
			Graph<ParserState>::Traversor vvalueStartState = ParserStateAutomata.AddVertex(valueStart, vnameEndState);
			Graph<ParserState>::Traversor vvalueEndState = ParserStateAutomata.AddVertex(valueEnd, vvalueStartState);
			Graph<ParserState>::Traversor vectorEndState = ParserStateAutomata.AddVertex(vectorEnd, vvalueEndState);
			ParserStateAutomata.CreateEdge(vectorEndState, endRouterState);

			// scope_start -> vector_start -> vector_end
			// vector_end -> end_state_router
			Graph<ParserState>::Traversor matStartState = ParserStateAutomata.AddVertex(matStart, scopeStartState);
			Graph<ParserState>::Traversor mnameStartState = ParserStateAutomata.AddVertex(nameStart, matStartState);
			Graph<ParserState>::Traversor mnameEndState = ParserStateAutomata.AddVertex(nameEnd, mnameStartState);
			Graph<ParserState>::Traversor mvalueStartState = ParserStateAutomata.AddVertex(valueStart, mnameEndState);
			Graph<ParserState>::Traversor mvalueEndState = ParserStateAutomata.AddVertex(valueEnd, mvalueStartState);
			Graph<ParserState>::Traversor matEndState = ParserStateAutomata.AddVertex(matEnd, mvalueEndState);
			ParserStateAutomata.CreateEdge(matEndState, endRouterState);

			// connect end_state_router to all possible start states
			ParserStateAutomata.CreateEdge(endRouterState, scopeStartState);
			ParserStateAutomata.CreateEdge(endRouterState, intStartState);
			ParserStateAutomata.CreateEdge(endRouterState, floatStartState);
			ParserStateAutomata.CreateEdge(endRouterState, stringStartState);
			ParserStateAutomata.CreateEdge(endRouterState, vectorStartState);
			ParserStateAutomata.CreateEdge(endRouterState, matStartState);

			// also connect end_state_router to the scope_end state
			ParserStateAutomata.CreateEdge(endRouterState, scopeEndState);

		}

		mStateTraversor = ParserStateAutomata.Begin();
	}

	XmlParseMaster::SharedData* SharedDataTable::Clone() const
	{
		return new SharedDataTable();
	}

	void SharedDataTable::ClearStateGraph()
	{
		//ParserStateAutomata.Clear();
	}

	bool SharedDataTable::CheckStateTransition(ParserState expectedState, bool selfTransitionAllowed)
	{
		bool hasValidStateTransition = false;

		if (selfTransitionAllowed)
		{
			if (*mStateTraversor == expectedState)
			{
				return true;
			}
		}
		mStateTraversor.ResetChildrenIterator();
		while (mStateTraversor.HasMoreChildren())
		{
			if (mStateTraversor.GetCurrentChildVertex() == expectedState)
			{
				hasValidStateTransition = true;
				mStateTraversor.TraverseToCurrentChild();
				break;
			}
			mStateTraversor.MoveToNextChild();
		}
		return hasValidStateTransition;
	}

	const Graph<SharedDataTable::ParserState>::Traversor & SharedDataTable::StateTraversor() const
	{
		return mStateTraversor;
	}

	Scope& SharedDataTable::RootScope()
	{
		return mRootScope;
	}

}
