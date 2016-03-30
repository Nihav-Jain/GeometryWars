#include "pch.h"
#include "ActionExpression.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionExpression);

	const std::string ActionExpression::ATTRIBUTE_EXPRESSION = "expression";

	ActionExpression::ActionExpression() :
		mPostfixExpression(nullptr)
	{
		Populate();
	}

	ActionExpression::ActionExpression(const ActionExpression& rhs) :
		mPostfixExpression(new SList<std::string>(*rhs.mPostfixExpression)), Action(rhs)
	{}

	ActionExpression::ActionExpression(ActionExpression&& rhs) :
		mPostfixExpression(rhs.mPostfixExpression), Action(std::move(rhs))
	{
		rhs.mPostfixExpression = nullptr;
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
			Stack<char> operatorStack;
			mPostfixExpression = new SList<std::string>();

			std::string allOperators = "=-+*/%";
			std::string trimDelimiter = " \f\n\r\t\v";

			std::string rawOperand;
			std::uint32_t prev = 0, pos;
			while ((pos = expression.find_first_of(allOperators, prev)) != std::string::npos)
			{
				if (pos > prev)
				{
					rawOperand = expression.substr(prev, pos - prev);
					rawOperand = TrimInplace(rawOperand);
					mPostfixExpression->PushBack(rawOperand);
				}

				char currentOperator = expression.at(pos);
				std::uint32_t currentOperatorIndex = allOperators.find(currentOperator);
				while (!operatorStack.IsEmpty())
				{
					if (allOperators.find(operatorStack.Top()) > currentOperatorIndex)
					{
						std::string s;
						s.push_back(operatorStack.Top());
						mPostfixExpression->PushBack(s);
						operatorStack.Pop();
					}
					else
					{
						break;
					}
				}
				operatorStack.Push(currentOperator);

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
				std::string s;
				s.push_back(operatorStack.Top());
				mPostfixExpression->PushBack(s);
				operatorStack.Pop();
			}

		}
	}

	void ActionExpression::EvaluateExpression()
	{
		Stack<std::string> evaluationStack;

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
