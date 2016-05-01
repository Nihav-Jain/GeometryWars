#pragma once
#include "XmlParseMaster.h"

namespace Library
{
	/**
	 *	Class to hold the parsed scope table, shared between all parse helpers
	 */
	class SharedDataTable : public XmlParseMaster::SharedData
	{
		RTTI_DECLARATIONS(SharedDataTable, XmlParseMaster::SharedData);
	public:

		/**
		 *	Enum to hold all possible states of the parser
		 */
		enum class ParserState
		{
			UNKNOWN,
			ROOT_STATE,
			STATE_ROUTER,
			WORLD_START,
			WORLD_END,
			SECTOR_START,
			SECTOR_END,
			ENTITY_START,
			ENTITY_END,
			ACTION_START,
			ACTION_END,
			SCOPE_START,
			SCOPE_END,
			PRIMITIVE_START,
			PRIMITIVE_END,
			NAME_START,
			NAME_END,
			VALUE_START,
			VALUE_END,
			SPRITE_START,
			SPRITE_END,
			//GAMEOBJECT_START,
			//GAMEOBJECT_END,
			END
		};

		/**
		 *	Default parameterless constructor for SharedDataTable
		 */
		SharedDataTable();

		/**
		 *	default destructor
		 */
		virtual ~SharedDataTable() = default;

		/**
		 *	disallow copy construction of SharedDataTable
		 */
		SharedDataTable(const SharedDataTable& rhs) = delete;

		/**
		 *	disallow copy assignment operator of SharedDataTable
		 */
		SharedDataTable& operator=(const SharedDataTable& rhs) = delete;

		/**
		 *	overriden Clone function to return a new SharedDataTable
		 */
		virtual SharedData* Clone() const override;

		/**
		 *	static function to clear the State Graph, to avoid memory leaks
		 */
		static void ClearStateGraph();

		/**
		 *	Checks if given state is a child state of current graph vertex, makes the transition to that state if true
		 *	@param state to which transition has to be made
		 *	@return true if the transition was made, false if no such transition is possible
		 */
		bool CheckStateTransition(ParserState expectedState);

		/**
		 *	Getter for the parser state graph traversor
		 *	@return constant reference to the state graph traversor
		 */
		const Graph<ParserState>::Traversor& StateTraversor() const;

		/**
		 *	Getter for root scope, the parent container of the parsed table
		 *	@return reference to a scope
		 */
		Scope& RootScope();

		/**
		 *	Setter for root scope, the parent container of the parsed table
		 *	@param reference to a Scope
		 */
		void SetRootScope(Scope& rootScope);

		/**
		 *	string to store the name of the datum variable when the xml syntax is of the form <name>variableName</name>
		 */
		std::string DataName;

		/**
		 *	string to store the value of the datum variable when the xml syntax is of the form <value>valueOfDatum</value>
		 */
		std::string DataValue;

		/**
		 *	boolean to indicate if the xml syntax contained <name> or <value> tag
		 */
		bool NameValueElementDataParsed;

	private:
		/**
		 *	Root (parent) of the scope table heirarchy
		 */
		Scope* mRootScope;

		/**
		 *	A graph to maintain the parsing state diagram
		 */
		static Graph<ParserState> ParserStateAutomata;

		/**
		 *	Traversor to manage the current vertex and possible transitions of the state diagram
		 */
		Graph<ParserState>::Traversor mStateTraversor;

	public:
		/**
		 *	Declared after RootScope because it is initialized with the pointer of RootScope and 
		 *	the constructors initialization line runs in the order of declaration
		 */
		Scope* CurrentScopePtr;
	};

}

