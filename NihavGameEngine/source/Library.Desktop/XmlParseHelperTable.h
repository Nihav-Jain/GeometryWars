#pragma once
#include "IXmlParseHelper.h"
#include "XmlParseMaster.h"
#include "Datum.h"
#include "Scope.h"
#include "Stack.h"
#include "Hashmap.h"
#include "Graph.h"

namespace Library
{
	class XmlParseHelperTable : public IXmlParseHelper
	{
	public:

		class SharedDataTable : public XmlParseMaster::SharedData
		{
			RTTI_DECLARATIONS(SharedDataTable, XmlParseMaster::SharedData);
		public:

			enum class ParserState
			{
				UNKNOWN,
				ROOT_STATE,
				END_STATE_ROUTER,
				SCOPE_START,
				SCOPE_END,
				INTEGER_START,
				INTEGER_END,
				FLOAT_START,
				FLOAT_END,
				STRING_START,
				STRING_END,
				NAME_START,
				NAME_END,
				VALUE_START,
				VALUE_END,
				VECTOR_START,
				VECTOR_END,
				MATRIX_START,
				MATRIX_END,
				END
			};

			SharedDataTable();
			SharedDataTable(const SharedDataTable& rhs) = delete;
			virtual ~SharedDataTable();

			SharedDataTable& operator=(const SharedDataTable& rhs) = delete;

			virtual SharedData* Clone() const override;
			static void ClearStateGraph();
			bool CheckStateTransition(ParserState expectedState, bool selfTransitionAllowed = false);

			Stack<Scope*> ScopeStack;
			Stack<std::string> ParsedElements;

			static Graph<ParserState> ParserStateAutomata;
			Graph<ParserState>::Traversor StateTraversor;
			Scope RootScope;
		private:

		};

		XmlParseHelperTable();
		XmlParseHelperTable(const XmlParseHelperTable& rhs) = delete;
		virtual ~XmlParseHelperTable();

		XmlParseHelperTable& operator=(const XmlParseHelperTable& rhs) = delete;

		virtual void Initialize() override;
		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName, const Hashmap<std::string, std::string>& attributes) override;
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& elementName) override;
		virtual IXmlParseHelper* Clone() const override;
		
	private:
	};
}

