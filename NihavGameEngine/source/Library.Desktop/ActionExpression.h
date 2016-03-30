#pragma once
#include "Action.h"

namespace Library
{
	class ActionExpression : public Action
	{
		RTTI_DECLARATIONS(ActionExpression, Action);
	public:
		ActionExpression();
		ActionExpression(const ActionExpression& rhs);
		ActionExpression(ActionExpression&& rhs);

		virtual ~ActionExpression();

		ActionExpression& operator=(const ActionExpression& rhs);
		ActionExpression& operator=(ActionExpression&& rhs);

		virtual void PostParsingProcess() override;
		virtual void Update(WorldState& worldState) override;

		virtual Scope* Clone(const Scope& rhs) const override;

		static const std::string ATTRIBUTE_EXPRESSION;

		SList<std::string>* mPostfixExpression;

	protected:
		virtual void DefinePrescribedAttributes() override;

	private:
		void ConvertExpressionToPostfix();
		void EvaluateExpression();

		Hashmap<std::string, std::uint32_t> mOperatorPrecedence;

		struct FunctionDefinition
		{
			std::uint32_t NumParams;
			// function pointer or function object to the corresponding function
		};
		Hashmap<std::string, FunctionDefinition> mDefinedFunctions;

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

