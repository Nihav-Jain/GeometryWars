#pragma once
#include "Action.h"

namespace Library
{
	/**
	 *	Evaluates an arithmetic expresison, currently handles =, +, -, *, /
	 *	Has 2 prescribed attributes - name (STRING) and expression (STRING)
	 *	@grammar <expression name="exp1"> <string name="expression"> result = A + (B - C) / D</string> </expression>
	 *	@requires XmlParseHelperActionExpresison, ActionExpressionFactory
	 *	@inherits Action
	 */
	class ActionExpression : public Action
	{
		RTTI_DECLARATIONS(ActionExpression, Action);
	public:

		/**
		 *	Constructor - initializes member variables and declares prescribed attributes
		 */
		ActionExpression();

		/**
		 *	disallow copy construtor
		 */
		ActionExpression(const ActionExpression& rhs) = delete;

		/**
		 *	disallow copy assignment operator
		 */
		ActionExpression& operator=(const ActionExpression& rhs) = delete;

		/**
		 *	disallow copy construtor
		 */
		virtual ~ActionExpression();

		/**
		 *	parses the expression and converts it from infix to postfix notation
		 *	called by the Action parser when the end tag is excountered
		 */
		virtual void PostParsingProcess() override;

		/**
		 *	Evaluates the expression
		 *	@param reference to the world state
		 */
		virtual void Update(WorldState& worldState) override;

		static const std::string ATTRIBUTE_EXPRESSION;

	private:
		void ConvertExpressionToPostfix();
		void EvaluateExpression();

		SList<std::string>* mPostfixExpression;

		static const Hashmap<std::string, std::uint32_t> mOperatorPrecedence;

		struct FunctionDefinition
		{
			std::uint32_t NumParams;
			// function pointer or function object to the corresponding function
		};
		Hashmap<std::string, FunctionDefinition> mDefinedFunctions;

		Datum Add(Datum& lhs, Datum& rhs);
		Datum Subtract(Datum& lhs, Datum& rhs);
		Datum Multiply(Datum& lhs, Datum& rhs);
		Datum Divide(Datum& lhs, Datum& rhs);
		Datum Assign(Datum& lhs, Datum& rhs);

		Datum LessThan(Datum& lhs, Datum& rhs);
		Datum GreaterThan(Datum& lhs, Datum& rhs);
		Datum LessThanEqualTo(Datum& lhs, Datum& rhs);
		Datum GreaterThanEqualTo(Datum& lhs, Datum& rhs);
		Datum Or(Datum& lhs, Datum& rhs);
		Datum And(Datum& lhs, Datum& rhs);
		Datum Equals(Datum& lhs, Datum& rhs);
		Datum NotEquals(Datum& lhs, Datum& rhs);

		typedef Datum(ActionExpression::*Arithmetic)(Datum&, Datum&);
		static const Hashmap<std::string, Arithmetic> mOperations;


		/**
		 *	trims the delimiter string from the end of given string
		 *	@param string to trim
		 * 	@param delimiter to trim, default value is a string containing all whitespace characters
		 */
		std::string& TrimRightInplace(std::string& s, const std::string& delimiters = " \f\n\r\t\v");

		/**
		 *	trims the delimiter string from the start of given string
		 *	@param string to trim
		 * 	@param delimiter to trim, default value is a string containing all whitespace characters
		 */
		std::string& TrimLeftInplace(std::string& s, const std::string& delimiters = " \f\n\r\t\v");

		/**
		 *	trims the given string based on the delimiter string
		 *	@param string to trim
		 * 	@param delimiter to trim, default value is a string containing all whitespace characters
		 */
		std::string& TrimInplace(std::string& s, const std::string& delimiters = " \f\n\r\t\v");

	};

	CONCRETE_ACTION_FACTORY(ActionExpression);

}

