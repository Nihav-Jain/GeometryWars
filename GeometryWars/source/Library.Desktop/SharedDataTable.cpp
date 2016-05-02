#include "pch.h"
#include "SharedDataTable.h"


namespace Library
{
	RTTI_DEFINITIONS(SharedDataTable, XmlParseMaster::SharedData);

	Graph<SharedDataTable::ParserState> SharedDataTable::ParserStateAutomata;

	SharedDataTable::SharedDataTable() :
		mRootScope(nullptr), CurrentScopePtr(nullptr), DataName(), DataValue(), NameValueElementDataParsed(false), mStateTraversor()
	{

		// prepare the state diagram graph here
		if (ParserStateAutomata.IsEmpty())
		{
			ParserState root = ParserState::ROOT_STATE;
			ParserState stateRouter = ParserState::STATE_ROUTER;

			ParserState worldStart = ParserState::WORLD_START;
			ParserState worldEnd = ParserState::WORLD_END;

			ParserState sectorStart = ParserState::SECTOR_START;
			ParserState sectorEnd = ParserState::SECTOR_END;

			ParserState entityStart = ParserState::ENTITY_START;
			ParserState entityEnd = ParserState::ENTITY_END;

			ParserState actionStart = ParserState::ACTION_START;
			ParserState actionEnd = ParserState::ACTION_END;
			
			ParserState scopeStart = ParserState::SCOPE_START;
			ParserState scopeEnd = ParserState::SCOPE_END;
			
			ParserState primitiveStart = ParserState::PRIMITIVE_START;
			ParserState primitiveEnd = ParserState::PRIMITIVE_END;
			
			ParserState nameStart = ParserState::NAME_START;
			ParserState nameEnd = ParserState::NAME_END;
			
			ParserState valueStart = ParserState::VALUE_START;
			ParserState valueEnd = ParserState::VALUE_END;

			//ParserState gameObjectStart = ParserState::GAMEOBJECT_START;
			//ParserState gameObjectEnd = ParserState::GAMEOBJECT_END;

			Graph<ParserState>::Traversor rootState = ParserStateAutomata.AddVertex(root);

			/** World */
			// root -> world_start -> world_router -> world_end
			Graph<ParserState>::Traversor worldStartState = ParserStateAutomata.AddVertex(worldStart, rootState);
			Graph<ParserState>::Traversor worldRouterState = ParserStateAutomata.AddVertex(stateRouter, worldStartState);
			Graph<ParserState>::Traversor worldEndState = ParserStateAutomata.AddVertex(worldEnd, worldRouterState);
			ParserStateAutomata.CreateEdge(worldEndState, worldStartState);

			// world_router -> prim_start -> name_start -> name_end -> value_start -> value_end -> prim_end -> world_router
			Graph<ParserState>::Traversor wPrimitiveStartState = ParserStateAutomata.AddVertex(primitiveStart, worldRouterState);
			Graph<ParserState>::Traversor wNameStart = ParserStateAutomata.AddVertex(nameStart, wPrimitiveStartState);
			Graph<ParserState>::Traversor wNameEnd = ParserStateAutomata.AddVertex(nameEnd, wNameStart);
			Graph<ParserState>::Traversor wValueStart = ParserStateAutomata.AddVertex(valueStart, wNameEnd);
			Graph<ParserState>::Traversor wValueEnd = ParserStateAutomata.AddVertex(valueEnd, wValueStart);
			Graph<ParserState>::Traversor wPrimitiveEndState = ParserStateAutomata.AddVertex(primitiveEnd, wValueEnd);
			ParserStateAutomata.CreateEdge(wPrimitiveEndState, worldRouterState);

			// world_router -> scope_start -> world_router
			Graph<ParserState>::Traversor wscopeStartState = ParserStateAutomata.AddVertex(scopeStart, worldRouterState);
			ParserStateAutomata.CreateEdge(wscopeStartState, worldRouterState);

			// world_router -> scope_end -> world_router
			Graph<ParserState>::Traversor wscopeEndState = ParserStateAutomata.AddVertex(scopeEnd, worldRouterState);
			ParserStateAutomata.CreateEdge(wscopeEndState, worldRouterState);
		
			/** Sector */
			// world_router -> sector_start -> sector_router -> sector_end -> world_router
			Graph<ParserState>::Traversor sectorStartState = ParserStateAutomata.AddVertex(sectorStart, worldRouterState);
			Graph<ParserState>::Traversor sectorRouterState = ParserStateAutomata.AddVertex(stateRouter, sectorStartState);
			Graph<ParserState>::Traversor sectorEndState = ParserStateAutomata.AddVertex(sectorEnd, sectorRouterState);
			ParserStateAutomata.CreateEdge(sectorEndState, worldRouterState);

			// sector_router -> prim_start -> name_start -> name_end -> value_start -> value_end -> prim_end -> sector_router
			Graph<ParserState>::Traversor sPrimitiveStartState = ParserStateAutomata.AddVertex(primitiveStart, sectorRouterState);
			Graph<ParserState>::Traversor sNameStart = ParserStateAutomata.AddVertex(nameStart, sPrimitiveStartState);
			Graph<ParserState>::Traversor sNameEnd = ParserStateAutomata.AddVertex(nameEnd, sNameStart);
			Graph<ParserState>::Traversor sValueStart = ParserStateAutomata.AddVertex(valueStart, sNameEnd);
			Graph<ParserState>::Traversor sValueEnd = ParserStateAutomata.AddVertex(valueEnd, sValueStart);
			Graph<ParserState>::Traversor sPrimitiveEndState = ParserStateAutomata.AddVertex(primitiveEnd, sValueEnd);
			ParserStateAutomata.CreateEdge(sPrimitiveEndState, sectorRouterState);

			// sector_router -> scope_start -> sector_router
			Graph<ParserState>::Traversor sScopeStartState = ParserStateAutomata.AddVertex(scopeStart, sectorRouterState);
			ParserStateAutomata.CreateEdge(sScopeStartState, sectorRouterState);

			// sector_router -> scope_end -> sector_router
			Graph<ParserState>::Traversor sScopeEndState = ParserStateAutomata.AddVertex(scopeEnd, sectorRouterState);
			ParserStateAutomata.CreateEdge(sScopeEndState, sectorRouterState);

			/** Entity */
			// sector_router -> entity_start -> entity_router -> entity_end -> sector_router
			Graph<ParserState>::Traversor entityStartState = ParserStateAutomata.AddVertex(entityStart, sectorRouterState);
			Graph<ParserState>::Traversor entityRouterState = ParserStateAutomata.AddVertex(stateRouter, entityStartState);
			Graph<ParserState>::Traversor entityEndState = ParserStateAutomata.AddVertex(entityEnd, entityRouterState);
			ParserStateAutomata.CreateEdge(entityEndState, sectorRouterState);

			// entity_router -> prim_start -> name_start -> name_end -> value_start -> value_end -> prim_end -> entity_router
			Graph<ParserState>::Traversor ePrimitiveStartState = ParserStateAutomata.AddVertex(primitiveStart, entityRouterState);
			Graph<ParserState>::Traversor eNameStart = ParserStateAutomata.AddVertex(nameStart, ePrimitiveStartState);
			Graph<ParserState>::Traversor eNameEnd = ParserStateAutomata.AddVertex(nameEnd, eNameStart);
			Graph<ParserState>::Traversor eValueStart = ParserStateAutomata.AddVertex(valueStart, eNameEnd);
			Graph<ParserState>::Traversor eValueEnd = ParserStateAutomata.AddVertex(valueEnd, eValueStart);
			Graph<ParserState>::Traversor ePrimitiveEndState = ParserStateAutomata.AddVertex(primitiveEnd, eValueEnd);
			ParserStateAutomata.CreateEdge(ePrimitiveEndState, entityRouterState);

			// entity_router -> scope_start -> entity_router
			Graph<ParserState>::Traversor eScopeStartState = ParserStateAutomata.AddVertex(scopeStart, entityRouterState);
			ParserStateAutomata.CreateEdge(eScopeStartState, entityRouterState);

			// entity_router -> scope_end -> entity_router
			Graph<ParserState>::Traversor eScopeEndState = ParserStateAutomata.AddVertex(scopeEnd, entityRouterState);
			ParserStateAutomata.CreateEdge(eScopeEndState, entityRouterState);

			/** Action */
			// entity_router -> action_start -> action_router -> action_end -> entity_router
			Graph<ParserState>::Traversor eActionStartState = ParserStateAutomata.AddVertex(actionStart, entityRouterState);
			Graph<ParserState>::Traversor eActionStateRouter = ParserStateAutomata.AddVertex(stateRouter, eActionStartState);
			Graph<ParserState>::Traversor eActionEndState = ParserStateAutomata.AddVertex(actionEnd, eActionStateRouter);
			ParserStateAutomata.CreateEdge(eActionEndState, entityRouterState);
			ParserStateAutomata.CreateEdge(eActionStateRouter, eActionStartState);
			ParserStateAutomata.CreateEdge(entityRouterState, eActionEndState);

			// action_router -> prim_start -> name_start -> name_end -> value_start -> value_end -> prim_end -> action_router
			Graph<ParserState>::Traversor aPrimitiveStartState = ParserStateAutomata.AddVertex(primitiveStart, eActionStateRouter);
			Graph<ParserState>::Traversor aNameStart = ParserStateAutomata.AddVertex(nameStart, aPrimitiveStartState);
			Graph<ParserState>::Traversor aNameEnd = ParserStateAutomata.AddVertex(nameEnd, aNameStart);
			Graph<ParserState>::Traversor aValueStart = ParserStateAutomata.AddVertex(valueStart, aNameEnd);
			Graph<ParserState>::Traversor aValueEnd = ParserStateAutomata.AddVertex(valueEnd, aValueStart);
			Graph<ParserState>::Traversor aPrimitiveEndState = ParserStateAutomata.AddVertex(primitiveEnd, aValueEnd);
			ParserStateAutomata.CreateEdge(aPrimitiveEndState, eActionStateRouter);

			// action_router -> scope_start -> action_router
			Graph<ParserState>::Traversor aScopeStartState = ParserStateAutomata.AddVertex(scopeStart, eActionStateRouter);
			ParserStateAutomata.CreateEdge(aScopeStartState, eActionStateRouter);

			// action_router -> scope_end -> action_router
			Graph<ParserState>::Traversor aScopeEndState = ParserStateAutomata.AddVertex(scopeEnd, eActionStateRouter);
			ParserStateAutomata.CreateEdge(aScopeEndState, eActionStateRouter);

			/** Sector - Action */
			// sector_router -> action_start -> action_router -> action_end -> sector_router
			Graph<ParserState>::Traversor sActionStartState = ParserStateAutomata.AddVertex(actionStart, sectorRouterState);
			Graph<ParserState>::Traversor sActionStateRouter = ParserStateAutomata.AddVertex(stateRouter, sActionStartState);
			Graph<ParserState>::Traversor sActionEndState = ParserStateAutomata.AddVertex(actionEnd, sActionStateRouter);
			ParserStateAutomata.CreateEdge(sActionEndState, sectorRouterState);
			ParserStateAutomata.CreateEdge(sActionStateRouter, sActionStartState);
			ParserStateAutomata.CreateEdge(sectorRouterState, sActionEndState);
			
			// action_router -> prim_start -> name_start -> name_end -> value_start -> value_end -> prim_end -> action_router
			Graph<ParserState>::Traversor saPrimitiveStartState = ParserStateAutomata.AddVertex(primitiveStart, sActionStateRouter);
			Graph<ParserState>::Traversor saNameStart = ParserStateAutomata.AddVertex(nameStart, saPrimitiveStartState);
			Graph<ParserState>::Traversor saNameEnd = ParserStateAutomata.AddVertex(nameEnd, saNameStart);
			Graph<ParserState>::Traversor saValueStart = ParserStateAutomata.AddVertex(valueStart, saNameEnd);
			Graph<ParserState>::Traversor saValueEnd = ParserStateAutomata.AddVertex(valueEnd, saValueStart);
			Graph<ParserState>::Traversor saPrimitiveEndState = ParserStateAutomata.AddVertex(primitiveEnd, saValueEnd);
			ParserStateAutomata.CreateEdge(saPrimitiveEndState, sActionStateRouter);

			// action_router -> scope_start -> action_router
			Graph<ParserState>::Traversor saScopeStartState = ParserStateAutomata.AddVertex(scopeStart, sActionStateRouter);
			ParserStateAutomata.CreateEdge(saScopeStartState, sActionStateRouter);

			// action_router -> scope_end -> action_router
			Graph<ParserState>::Traversor saScopeEndState = ParserStateAutomata.AddVertex(scopeEnd, sActionStateRouter);
			ParserStateAutomata.CreateEdge(saScopeEndState, sActionStateRouter);

			/** World - Action */
			// world_router -> action_start -> action_router -> action_end -> world_router
			Graph<ParserState>::Traversor wActionStartState = ParserStateAutomata.AddVertex(actionStart, worldRouterState);
			Graph<ParserState>::Traversor wActionStateRouter = ParserStateAutomata.AddVertex(stateRouter, wActionStartState);
			Graph<ParserState>::Traversor wActionEndState = ParserStateAutomata.AddVertex(actionEnd, wActionStateRouter);
			ParserStateAutomata.CreateEdge(wActionEndState, worldRouterState);
			ParserStateAutomata.CreateEdge(wActionStateRouter, wActionStartState);
			ParserStateAutomata.CreateEdge(worldRouterState, wActionEndState);

			// action_router -> prim_start -> name_start -> name_end -> value_start -> value_end -> prim_end -> action_router
			Graph<ParserState>::Traversor waPrimitiveStartState = ParserStateAutomata.AddVertex(primitiveStart, wActionStateRouter);
			Graph<ParserState>::Traversor waNameStart = ParserStateAutomata.AddVertex(nameStart, waPrimitiveStartState);
			Graph<ParserState>::Traversor waNameEnd = ParserStateAutomata.AddVertex(nameEnd, waNameStart);
			Graph<ParserState>::Traversor waValueStart = ParserStateAutomata.AddVertex(valueStart, waNameEnd);
			Graph<ParserState>::Traversor waValueEnd = ParserStateAutomata.AddVertex(valueEnd, waValueStart);
			Graph<ParserState>::Traversor waPrimitiveEndState = ParserStateAutomata.AddVertex(primitiveEnd, waValueEnd);
			ParserStateAutomata.CreateEdge(waPrimitiveEndState, wActionStateRouter);

			// action_router -> scope_start -> action_router
			Graph<ParserState>::Traversor waScopeStartState = ParserStateAutomata.AddVertex(scopeStart, wActionStateRouter);
			ParserStateAutomata.CreateEdge(waScopeStartState, wActionStateRouter);

			// action_router -> scope_end -> action_router
			Graph<ParserState>::Traversor waScopeEndState = ParserStateAutomata.AddVertex(scopeEnd, wActionStateRouter);
			ParserStateAutomata.CreateEdge(waScopeEndState, wActionStateRouter);

			/** GameObject */ // Removing because it doesn't work and we have no idea why
			//// sector_router -> gameObject_start -> gameObject_router -> gameObject_end -> sector_router
			//Graph<ParserState>::Traversor gameObjectStartState = ParserStateAutomata.AddVertex(gameObjectStart, sectorRouterState);
			//Graph<ParserState>::Traversor gameObjectRouterState = ParserStateAutomata.AddVertex(stateRouter, gameObjectStartState);
			//Graph<ParserState>::Traversor gameObjectEndState = ParserStateAutomata.AddVertex(gameObjectEnd, gameObjectRouterState);
			//ParserStateAutomata.CreateEdge(gameObjectEndState, sectorRouterState);

			//// gameObject_router -> prim_start -> name_start -> name_end -> value_start -> value_end -> prim_end -> gameObject_router
			//Graph<ParserState>::Traversor goPrimitiveStartState = ParserStateAutomata.AddVertex(primitiveStart, gameObjectRouterState);
			//Graph<ParserState>::Traversor goNameStart = ParserStateAutomata.AddVertex(nameStart, goPrimitiveStartState);
			//Graph<ParserState>::Traversor goNameEnd = ParserStateAutomata.AddVertex(nameEnd, goNameStart);
			//Graph<ParserState>::Traversor goValueStart = ParserStateAutomata.AddVertex(valueStart, goNameEnd);
			//Graph<ParserState>::Traversor goValueEnd = ParserStateAutomata.AddVertex(valueEnd, goValueStart);
			//Graph<ParserState>::Traversor goPrimitiveEndState = ParserStateAutomata.AddVertex(primitiveEnd, goValueEnd);
			//ParserStateAutomata.CreateEdge(goPrimitiveEndState, gameObjectRouterState);

			//// gameObject_router -> scope_start -> gameObject_router
			//Graph<ParserState>::Traversor goScopeStartState = ParserStateAutomata.AddVertex(scopeStart, gameObjectRouterState);
			//ParserStateAutomata.CreateEdge(goScopeStartState, gameObjectRouterState);

			//// gameObject_router -> scope_end -> gameObject_router
			//Graph<ParserState>::Traversor goScopeEndState = ParserStateAutomata.AddVertex(scopeEnd, gameObjectRouterState);
			//ParserStateAutomata.CreateEdge(goScopeEndState, gameObjectRouterState);

			//// gameObject_router -> action_start -> action_router -> action_end -> gameObject_router
			//Graph<ParserState>::Traversor goActionStartState = ParserStateAutomata.AddVertex(actionStart, gameObjectRouterState);
			//Graph<ParserState>::Traversor goActionStateRouter = ParserStateAutomata.AddVertex(stateRouter, goActionStartState);
			//Graph<ParserState>::Traversor goActionEndState = ParserStateAutomata.AddVertex(actionEnd, goActionStateRouter);
			//ParserStateAutomata.CreateEdge(goActionEndState, gameObjectRouterState);
			//ParserStateAutomata.CreateEdge(goActionStateRouter, goActionStartState);
			//ParserStateAutomata.CreateEdge(gameObjectRouterState, goActionEndState);
		}

		mStateTraversor = ParserStateAutomata.Begin();
	}

	XmlParseMaster::SharedData* SharedDataTable::Clone() const
	{
		return new SharedDataTable();
	}

	void SharedDataTable::ClearStateGraph()
	{
		ParserStateAutomata.Clear();
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
		return *mRootScope;
	}

	void SharedDataTable::SetRootScope(Scope& rootScope)
	{
		mRootScope = &rootScope;
		CurrentScopePtr = mRootScope;
	}

}
