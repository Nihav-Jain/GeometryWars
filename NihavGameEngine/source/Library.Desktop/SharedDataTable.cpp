#include "pch.h"
#include "SharedDataTable.h"


namespace Library
{
	RTTI_DEFINITIONS(SharedDataTable);

	//Graph<SharedDataTable::ParserState> SharedDataTable::ParserStateAutomata;

	SharedDataTable::SharedDataTable() :
		mRootScope(), CurrentScopePtr(&mRootScope), DataName(), DataValue(), NameValueElementDataParsed(false), mStateTraversor()
	{

		// prepare the state diagram graph here
		if (ParserStateAutomata.IsEmpty())
		{
			ParserState root = ParserState::ROOT_STATE;
			ParserState endRouter = ParserState::END_STATE_ROUTER;
			ParserState scopeStart = ParserState::SCOPE_START;
			ParserState scopeEnd = ParserState::SCOPE_END;
			ParserState primitiveStart = ParserState::PRIMITIVE_START;
			ParserState primitiveEnd = ParserState::PRIMITIVE_END;
			ParserState nameStart = ParserState::NAME_START;
			ParserState nameEnd = ParserState::NAME_END;
			ParserState valueStart = ParserState::VALUE_START;
			ParserState valueEnd = ParserState::VALUE_END;


			// root -> end_state_router
			// root -> scope_start -> scope_end -> end_state_router
			Graph<ParserState>::Traversor rootState = ParserStateAutomata.AddVertex(root);
			Graph<ParserState>::Traversor scopeStartState = ParserStateAutomata.AddVertex(scopeStart, rootState);
			Graph<ParserState>::Traversor endRouterState = ParserStateAutomata.AddVertex(endRouter, rootState);
			Graph<ParserState>::Traversor scopeEndState = ParserStateAutomata.AddVertex(scopeEnd, scopeStartState);
			ParserStateAutomata.CreateEdge(scopeEndState, endRouterState);
			ParserStateAutomata.CreateEdge(scopeStartState, scopeStartState);

			// scope_start -> primitive_start -> name_start -> name_end -> value_start -> value_end -> primitive_end
			// primitive_start -> end_state_router
			Graph<ParserState>::Traversor primStartState = ParserStateAutomata.AddVertex(primitiveStart, scopeStartState);
			Graph<ParserState>::Traversor nameStartState = ParserStateAutomata.AddVertex(nameStart, primStartState);
			Graph<ParserState>::Traversor nameEndState = ParserStateAutomata.AddVertex(nameEnd, nameStartState);
			Graph<ParserState>::Traversor valueStartState = ParserStateAutomata.AddVertex(valueStart, nameEndState);
			Graph<ParserState>::Traversor valueEndState = ParserStateAutomata.AddVertex(valueEnd, valueStartState);
			Graph<ParserState>::Traversor primEndState = ParserStateAutomata.AddVertex(primitiveEnd, valueEndState);
			ParserStateAutomata.CreateEdge(primEndState, endRouterState);

			// connect end_state_router to all possible start states
			ParserStateAutomata.CreateEdge(endRouterState, scopeStartState);
			ParserStateAutomata.CreateEdge(endRouterState, primStartState);

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

	bool SharedDataTable::CheckStateTransition(ParserState expectedState)
	{
		bool hasValidStateTransition = false;

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
