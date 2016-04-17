#include "pch.h"
#include "ActionExpression.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionExpression);

	const std::string ActionExpression::ATTRIBUTE_EXPRESSION = "expression";
	const Hashmap<std::string, std::uint32_t> ActionExpression::mOperatorPrecedence = {
		{ "=",	1 },
		{ "||", 2 },
		{ "&&", 3 },
		{ "==", 4 },
		{ "!=", 4 },
		{ "<",	5 },
		{ "<=", 5 },
		{ ">",	5 },
		{ ">=", 5 },
		{ "+",	6 },
		{ "-",	6 },
		{ "*",	7 },
		{ "/",	7 }
	};

	const Hashmap<std::string, ActionExpression::Arithmetic> ActionExpression::mOperations = {
		{ "=",  &ActionExpression::Assign },
		{ "+",  &ActionExpression::Add },
		{ "-",  &ActionExpression::Subtract },
		{ "*",  &ActionExpression::Multiply },
		{ "/",  &ActionExpression::Divide },
		{ "<",  &ActionExpression::LessThan },
		{ ">",  &ActionExpression::GreaterThan },
		{ "<=", &ActionExpression::LessThanEqualTo },
		{ ">=", &ActionExpression::GreaterThanEqualTo },
		{ "&&", &ActionExpression::And },
		{ "||", &ActionExpression::Or },
		{ "==", &ActionExpression::Equals },
		{ "!=", &ActionExpression::NotEquals }
	};

	ActionExpression::ActionExpression() :
		mPostfixExpression(nullptr), mDefinedFunctions()
	{
		AddInternalAttribute(ATTRIBUTE_EXPRESSION, "");

		
		mDefinedFunctions["max"].NumParams = 2;
		mDefinedFunctions["min"].NumParams = 2;
		mDefinedFunctions["sin"].NumParams = 1;
		mDefinedFunctions["cos"].NumParams = 1;
		mDefinedFunctions["tan"].NumParams = 1;
		mDefinedFunctions["atan"].NumParams = 1;
		mDefinedFunctions["exp"].NumParams = 1;
		mDefinedFunctions["log"].NumParams = 1;
		mDefinedFunctions["pow"].NumParams = 2;
		mDefinedFunctions["sqrt"].NumParams = 1;
	}

	ActionExpression::~ActionExpression()
	{
		if (mPostfixExpression != nullptr)
			delete mPostfixExpression;
	}

	void ActionExpression::PostParsingProcess()
	{
		ConvertExpressionToPostfix();
	}

	void ActionExpression::Update(WorldState& worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
		EvaluateExpression();
	}

	void ActionExpression::ConvertExpressionToPostfix()
	{
		std::string& expression = (*this)[ATTRIBUTE_EXPRESSION].Get<std::string>();
		if (!expression.empty())
		{
			Stack<std::string> operatorStack;
			mPostfixExpression = new SList<std::string>();

			std::string allOperators = "(),=-+*/&|<>!";
			std::string trimDelimiter = " \f\n\r\t\v";
			std::uint32_t indexOfComma = (std::uint32_t)allOperators.find(',');

			std::string rawOperand;
			std::uint32_t prev = 0, pos;
			while ((pos = (std::uint32_t)expression.find_first_of(allOperators, prev)) != std::string::npos)
			{
				if (pos > prev)
				{
					rawOperand = expression.substr(prev, pos - prev);
					rawOperand = TrimInplace(rawOperand);
					if (!rawOperand.empty())
					{
						if (mDefinedFunctions.ContainsKey(rawOperand))
							operatorStack.Push(rawOperand);
						else
							mPostfixExpression->PushBack(rawOperand);
					}
				}

				std::string currentOperator;
				currentOperator.push_back(expression.at(pos));

				// check if the operator has more than 1 characters
				// TODO: more rigourous checking
				if (allOperators.find(expression.at(pos)) > indexOfComma)
				{
					std::uint32_t nextCharacterIndex = (std::uint32_t)allOperators.find(expression.at(pos + 1));
					if (nextCharacterIndex != std::string::npos && nextCharacterIndex > indexOfComma)
					{
						pos++;
						currentOperator.push_back(expression.at(pos));
					}
					assert(mOperatorPrecedence.ContainsKey(currentOperator));
				}

				if (currentOperator == ",")
				{
					while (operatorStack.Top() != "(")
					{
						mPostfixExpression->PushBack(operatorStack.Top());
						operatorStack.Pop();
					}
				}
				else if (currentOperator == "(")
				{
					operatorStack.Push(currentOperator);
				}
				else if (currentOperator == ")")
				{
					while (operatorStack.Top() != "(")
					{
						mPostfixExpression->PushBack(operatorStack.Top());
						operatorStack.Pop();
					}
					operatorStack.Pop();
					if (!operatorStack.IsEmpty())
					{
						if (mDefinedFunctions.ContainsKey(operatorStack.Top()))
						{
							mPostfixExpression->PushBack(operatorStack.Top());
							operatorStack.Pop();
						}
					}
				}
				else
				{
					assert(mOperatorPrecedence.ContainsKey(currentOperator));
					uint32_t currentOperatorPrecedence = mOperatorPrecedence[currentOperator];
					if (!operatorStack.IsEmpty())
					{
						if (operatorStack.Top() != "(")
						{
							while (!operatorStack.IsEmpty())
							{
								if (currentOperatorPrecedence <= mOperatorPrecedence[operatorStack.Top()])
								{
									mPostfixExpression->PushBack(operatorStack.Top());
									operatorStack.Pop();
								}
								else
									break;
							}

						}
					}
					operatorStack.Push(currentOperator);
				}

				prev = pos + 1;
			}
			if (prev < expression.length())
			{
				rawOperand = expression.substr(prev, std::string::npos);
				rawOperand = TrimInplace(rawOperand);
				mPostfixExpression->PushBack(rawOperand);
			}

			while (!operatorStack.IsEmpty())
			{
				mPostfixExpression->PushBack(operatorStack.Top());
				operatorStack.Pop();
			}

		}
	}

	void ActionExpression::EvaluateExpression()
	{
		Stack<Datum*> evaluationStack;
		SList<std::string> postfixExpression(*mPostfixExpression);
		Stack<Datum*> resultDatums;
		Datum result;
		while (!postfixExpression.IsEmpty())
		{
			if (mOperatorPrecedence.ContainsKey(postfixExpression.Front()))
			{
				Datum* rhs = evaluationStack.Top();
				evaluationStack.Pop();
				Datum* lhs = evaluationStack.Top();
				evaluationStack.Pop();

				if (resultDatums.IsEmpty() || !((rhs == resultDatums.Top()) || (lhs == resultDatums.Top())))
					resultDatums.Push(new Datum());
				*resultDatums.Top() = (this->*mOperations[postfixExpression.Front()])(*lhs, *rhs);
				evaluationStack.Push(resultDatums.Top());
				postfixExpression.PopFront();
			}
			else if (mDefinedFunctions.ContainsKey(postfixExpression.Front()))
			{

			}
			else
			{
				Datum* operand = Search(postfixExpression.Front());
				assert(operand != nullptr);
				evaluationStack.Push(operand);
				postfixExpression.PopFront();
			}
		}

		while (!resultDatums.IsEmpty())
		{
			delete resultDatums.Top();
			resultDatums.Pop();
		}
	}

	Datum ActionExpression::Add(Datum& lhs, Datum& rhs)
	{
		return lhs + rhs;
	}

	Datum ActionExpression::Subtract(Datum& lhs, Datum& rhs)
	{
		return lhs - rhs;
	}

	Datum ActionExpression::Multiply(Datum& lhs, Datum& rhs)
	{
		return lhs * rhs;
	}

	Datum ActionExpression::Divide(Datum& lhs, Datum& rhs)
	{
		return lhs / rhs;
	}

	Datum ActionExpression::Assign(Datum& lhs, Datum& rhs)
	{
		lhs = rhs;
		return Datum();
	}

	Datum ActionExpression::LessThan(Datum& lhs, Datum& rhs)
	{
		return lhs < rhs;
	}

	Datum ActionExpression::GreaterThan(Datum& lhs, Datum& rhs)
	{
		return lhs > rhs;
	}

	Datum ActionExpression::LessThanEqualTo(Datum& lhs, Datum& rhs)
	{
		return lhs <= rhs;
	}

	Datum ActionExpression::GreaterThanEqualTo(Datum& lhs, Datum& rhs)
	{
		return lhs >= rhs;
	}

	Datum ActionExpression::Or(Datum& lhs, Datum& rhs)
	{
		return lhs || rhs;
	}

	Datum ActionExpression::And(Datum& lhs, Datum& rhs)
	{
		return lhs && rhs;
	}

	Datum ActionExpression::Equals(Datum& lhs, Datum& rhs)
	{
		Datum result;
		result = (lhs == rhs);
		return result;
	}

	Datum ActionExpression::NotEquals(Datum& lhs, Datum& rhs)
	{
		Datum result;
		result = (lhs != rhs);
		return result;
	}

	std::string& ActionExpression::TrimRightInplace(std::string& s, const std::string& delimiters)
	{
		return s.erase(s.find_last_not_of(delimiters) + 1);
	}

	std::string& ActionExpression::TrimLeftInplace(std::string& s, const std::string& delimiters)
	{
		return s.erase(0, s.find_first_not_of(delimiters));
	}

	std::string& ActionExpression::TrimInplace(std::string& s, const std::string& delimiters)
	{
		return TrimLeftInplace(TrimRightInplace(s, delimiters), delimiters);
	}
}
