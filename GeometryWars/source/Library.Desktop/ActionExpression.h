#pragma once
#include <functional>
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

		ActionExpression(const ActionExpression& rhs);
		ActionExpression(ActionExpression&& rhs);

		ActionExpression& operator=(const ActionExpression& rhs);
		ActionExpression& operator=(ActionExpression&& rhs);

		/**
		 *	disallow copy construtor
		 */
		virtual ~ActionExpression();

		virtual void BeginPlay(WorldState& worldState) override;

		/**
		 *	Evaluates the expression
		 *	@param reference to the world state
		 */
		virtual void Update(WorldState& worldState) override;
		
		virtual Scope* Clone(const Scope& rhs) const override;

		/**
		 *	Clears the static memebers to avoid memory leak detection in the Unit Tests
		 *	Its called in the destructor of Game
		 */
		static void ClearStaticMemebers();

		/**
		 *	Structure to maintain information about functions which have been defined to be used in expressions.
		 *	@param an unsigned integer representing the number of parameters this function expects
		 *	@param lambda expression which will be used as the funtions body; 
		 *	the definition of the lambda expression is -> return type - Datum; parameters -> const Vector<Datum>& (contains the list of parameters in the same order as defined in XML)
		 */
		struct FunctionDefinition
		{
			FunctionDefinition(std::uint32_t numParams, std::function<Datum(const Vector<Datum*>&)> functionBody) :
				NumParams(numParams), FunctionBody(std::move(functionBody))
			{}
			std::uint32_t NumParams;
			std::function<Datum(const Vector<Datum*>&)> FunctionBody;
		};
		typedef Hashmap<std::string, FunctionDefinition> CallableFunctions;

		struct RefFunctionDefinition
		{
			RefFunctionDefinition(std::uint32_t numParams, std::function<Datum*(const Vector<Datum*>&)> functionBody) :
				NumParams(numParams), FunctionBody(std::move(functionBody))
			{}
			std::uint32_t NumParams;
			std::function<Datum*(const Vector<Datum*>&)> FunctionBody;
		};
		typedef Hashmap<std::string, RefFunctionDefinition> CallableRefFunctions;


		/**
		 *	Adds a FunctionDefinition to the list of defined functions, this function can now be used in XML expressions
		 *	@param name of the function
		 *	@param FunctionDefinition (lambda expression for function body and number of params)
		 *	@return true if function was added successfully, false if there was already a function by this name
		 */
		static bool AddFunction(const std::string& functionName, FunctionDefinition functionDefinition);

		static bool AddRefFunction(const std::string& functionName, RefFunctionDefinition functionDefinition);

		/**
		 *	Checks if a function with the given name has already been defined or not
		 *	@param name of the function
		 *	@return true if a function with the given name already exists, false otherwise
		 */
		static bool IsFunctionDefined(const std::string& functionName);

		static const std::string ATTRIBUTE_EXPRESSION;

	private:
		void ConvertExpressionToPostfix();
		void EvaluateExpression();

		SList<std::string>* mPostfixExpression;

		static const Hashmap<std::string, std::uint32_t> mOperatorPrecedence;
		static CallableFunctions mDefinedFunctions;
		static CallableRefFunctions mDefinedRefFunctions;

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

		std::uint32_t mTempVariableCounter;

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

