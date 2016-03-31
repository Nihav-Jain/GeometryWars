#include "pch.h"
#include "ActionExpression.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionExpression);

	const std::string ActionExpression::ATTRIBUTE_EXPRESSION = "expression";

	ActionExpression::ActionExpression() :
		mPostfixExpression(nullptr), mOperatorPrecedence(), mDefinedFunctions(), mArithmeticOperations()
	{
		Populate();

		std::uint32_t i = 1;
		
		mOperatorPrecedence.Insert("=", i++);

		mOperatorPrecedence.Insert("+", i);
		mOperatorPrecedence.Insert("-", i++);

		mOperatorPrecedence.Insert("*", i);
		mOperatorPrecedence.Insert("/", i++);

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

		mArithmeticOperations["="] = &ActionExpression::Assign;
		mArithmeticOperations["+"] = &ActionExpression::Add;
		mArithmeticOperations["-"] = &ActionExpression::Subtract;
		mArithmeticOperations["*"] = &ActionExpression::Multiply;
		mArithmeticOperations["/"] = &ActionExpression::Divide;

	}

	ActionExpression::~ActionExpression()
	{
		if (mPostfixExpression != nullptr)
			delete mPostfixExpression;
	}

	ActionExpression& ActionExpression::operator=(const ActionExpression& rhs)
	{
		if (this != &rhs)
		{
			mPostfixExpression = new SList<std::string>(*rhs.mPostfixExpression);

			Action::operator=(rhs);
		}

		return *this;
	}

	ActionExpression& ActionExpression::operator=(ActionExpression&& rhs)
	{
		if (this != &rhs)
		{
			mPostfixExpression = rhs.mPostfixExpression;

			Action::operator=(std::move(rhs));

			rhs.mPostfixExpression = nullptr;
		}

		return *this;
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

	Scope* ActionExpression::Clone(const Scope& rhs) const
	{
		if (!rhs.Is(ActionExpression::TypeIdClass()))
			throw std::exception("Given Scope reference is not an ActionExpression.");
		return new ActionExpression(*(rhs.As<ActionExpression>()));
	}

	void ActionExpression::DefinePrescribedAttributes()
	{
		AddInternalAttribute(ATTRIBUTE_EXPRESSION, "");
	}

	void ActionExpression::ConvertExpressionToPostfix()
	{
		std::string& expression = (*this)[ATTRIBUTE_EXPRESSION].Get<std::string>();
		if (!expression.empty())
		{
			Stack<std::string> operatorStack;
			mPostfixExpression = new SList<std::string>();

			std::string allOperators = "(),=-+*/";
			std::string trimDelimiter = " \f\n\r\t\v";
			std::uint32_t indexOfComma = allOperators.find(',');

			std::string rawOperand;
			std::uint32_t prev = 0, pos;
			while ((pos = expression.find_first_of(allOperators, prev)) != std::string::npos)
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
					std::uint32_t nextCharacterIndex = allOperators.find(expression.at(pos + 1));
					if (nextCharacterIndex != std::string::npos && nextCharacterIndex > indexOfComma)
					{
						pos++;
						currentOperator.push_back(expression.at(pos));
					}
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

		Datum result;
		while (!mPostfixExpression->IsEmpty())
		{
			if (mOperatorPrecedence.ContainsKey(mPostfixExpression->Front()))
			{
				Datum* rhs = evaluationStack.Top();
				evaluationStack.Pop();
				Datum* lhs = evaluationStack.Top();
				evaluationStack.Pop();

				result = (this->*mArithmeticOperations[mPostfixExpression->Front()])(*lhs, *rhs);
				evaluationStack.Push(&result);
				mPostfixExpression->PopFront();
			}
			else if (mDefinedFunctions.ContainsKey(mPostfixExpression->Front()))
			{

			}
			else
			{
				Datum* operand = Search(mPostfixExpression->Front());
				assert(operand != nullptr);
				evaluationStack.Push(operand);
				mPostfixExpression->PopFront();
			}
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
