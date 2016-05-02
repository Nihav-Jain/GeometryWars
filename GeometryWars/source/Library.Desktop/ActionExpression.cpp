#include "pch.h"
#include "ActionExpression.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionExpression, Action);

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

	Hashmap<std::string, ActionExpression::FunctionDefinition> ActionExpression::mDefinedFunctions;

	ActionExpression::ActionExpression() :
		mPostfixExpression(nullptr), mTempVariableCounter(0)
	{
		AddInternalAttribute(ATTRIBUTE_EXPRESSION, "");
		
		mDefinedFunctions.Insert("max", FunctionDefinition(2, [](const Vector<Datum*>& params)
		{
			assert(params.Size() == 2);
			Datum result;
			result = ( (*params[0] >= *params[1]).Get<bool>() ) ? *params[0] : *params[1];
			return result;
		} ));

		mDefinedFunctions.Insert("arraySize", FunctionDefinition(1, [](const Vector<Datum*>& params)
		{
			assert(params.Size() == 1);
			Datum result;
			result = static_cast<std::int32_t>(params[0]->Size());
			return result;
		}));

		mDefinedFunctions.Insert("array", FunctionDefinition(2, [](const Vector<Datum*>& params)
		{
			assert(params.Size() == 2);
			Datum result;
			std::int32_t index = params[1]->Get<std::int32_t>();
			switch (params[0]->Type())
			{
			case Datum::DatumType::INTEGER:
				result = params[0]->Get<std::int32_t>(index);
				break;
			case Datum::DatumType::FLOAT:
				result = params[0]->Get<std::float_t>(index);
				break;
			case Datum::DatumType::STRING:
				result = params[0]->Get<std::string>(index);
				break;
			case Datum::DatumType::BOOLEAN:
				result = params[0]->Get<bool>(index);
				break;
			case Datum::DatumType::VECTOR4:
				result = params[0]->Get<glm::vec4>(index);
				break;
			case Datum::DatumType::MATRIX4x4:
				result = params[0]->Get<glm::mat4x4>(index);
				break;
			default:
				break;
			}
			return result;
		}));

		// param0 - destination array
		// param1 - destination array index
		// param2 - source array
		// param3 - source array index
		// returns an empty datum because it updates the value by reference
		mDefinedFunctions.Insert("set_array", FunctionDefinition(4, [](const Vector<Datum*>& params)
		{
			assert(params.Size() == 4);
			Datum result;
			std::int32_t destinationIndex = params[1]->Get<std::int32_t>();
			std::int32_t sourceIndex = params[3]->Get<std::int32_t>();
			switch (params[0]->Type())
			{
			case Datum::DatumType::INTEGER:
				params[0]->Set(params[2]->Get<std::int32_t>(sourceIndex), destinationIndex);
				break;
			case Datum::DatumType::FLOAT:
				params[0]->Set(params[2]->Get<std::float_t>(sourceIndex), destinationIndex);
				break;
			case Datum::DatumType::STRING:
				params[0]->Set(params[2]->Get<std::string>(sourceIndex), destinationIndex);
				break;
			case Datum::DatumType::BOOLEAN:
				params[0]->Set(params[2]->Get<bool>(sourceIndex), destinationIndex);
				break;
			case Datum::DatumType::VECTOR4:
				params[0]->Set(params[2]->Get<glm::vec4>(sourceIndex), destinationIndex);
				break;
			case Datum::DatumType::MATRIX4x4:
				params[0]->Set(params[2]->Get < glm::mat4x4> (sourceIndex), destinationIndex);
				break;
			default:
				break;
			}
			return result;
		}));

		/*mDefinedFunctions["max"].NumParams = 2;
		mDefinedFunctions["min"].NumParams = 2;
		mDefinedFunctions["sin"].NumParams = 1;
		mDefinedFunctions["cos"].NumParams = 1;
		mDefinedFunctions["tan"].NumParams = 1;
		mDefinedFunctions["atan"].NumParams = 1;
		mDefinedFunctions["exp"].NumParams = 1;
		mDefinedFunctions["log"].NumParams = 1;
		mDefinedFunctions["pow"].NumParams = 2;
		mDefinedFunctions["sqrt"].NumParams = 1;*/
	}

	ActionExpression::~ActionExpression()
	{
		if (mPostfixExpression != nullptr)
			delete mPostfixExpression;
	}

	void ActionExpression::BeginPlay(WorldState& worldState)
	{
		Action::BeginPlay(worldState);
		ConvertExpressionToPostfix();
	}

	void ActionExpression::Update(WorldState& worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
		EvaluateExpression(*worldState.world);
	}

	void ActionExpression::ClearStaticMemebers()
	{
		mDefinedFunctions.Clear();
	}

	bool ActionExpression::AddFunction(const std::string& functionName, FunctionDefinition functionDefinition)
	{
		bool didNewInsert = false;
		mDefinedFunctions.Insert(CallableFunctions::PairType(functionName, functionDefinition), didNewInsert);

		return didNewInsert;
	}

	bool ActionExpression::IsFunctionDefined(const std::string& functionName)
	{
		return mDefinedFunctions.ContainsKey(functionName);
	}

	// https://en.wikipedia.org/wiki/Shunting-yard_algorithm#The_algorithm_in_detail
	void ActionExpression::ConvertExpressionToPostfix()
	{
		std::string& expression = (*this)[ATTRIBUTE_EXPRESSION].Get<std::string>();
		if (!expression.empty())
		{
			Stack<std::string> operatorStack;
			mPostfixExpression = new SList<std::string>();

			std::string allOperators = "(){},=-+*/&|<>!";
			std::string trimDelimiter = " \f\n\r\t\v";
			std::uint32_t indexOfComma = (std::uint32_t)allOperators.find(',');

			std::string rawOperand;
			std::uint32_t prev = 0, pos;
			while ((pos = (std::uint32_t)expression.find_first_of(allOperators, prev)) < expression.length())
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
				if (allOperators.find(expression.at(pos)) > indexOfComma)
				{
					std::uint32_t nextCharacterIndex = (std::uint32_t)allOperators.find(expression.at(pos + 1));
					if (nextCharacterIndex < allOperators.length() && nextCharacterIndex > indexOfComma)
					{
						pos++;
						currentOperator.push_back(expression.at(pos));
					}
					assert(mOperatorPrecedence.ContainsKey(currentOperator));
				}

				if (currentOperator == "{")
				{
					prev = pos + 1;
					pos = (std::uint32_t)expression.find("}", prev);
					std::string literal = expression.substr(prev, pos - prev);
					std::string tempName = std::string("temp" + std::to_string(mTempVariableCounter));
					Datum& tempLiteral = AppendAuxiliaryAttribute(tempName);

					std::uint32_t quote = (std::uint32_t)literal.find("\"");
					if (quote < literal.length())
					{
						std::uint32_t endQuote = (std::uint32_t)literal.find_last_of("\"");
						assert(endQuote < literal.length());
						literal = literal.substr(quote + 1, endQuote - quote - 1);
						tempLiteral.SetType(Datum::DatumType::STRING);
					}
					else if (literal.find("vec4") < literal.length())
					{
						tempLiteral.SetType(Datum::DatumType::VECTOR4);
					}
					else if (literal.find("mat4x4") < literal.length())
					{
						tempLiteral.SetType(Datum::DatumType::MATRIX4x4);
					}
					else if (literal.find(".") < literal.length())
					{
						tempLiteral.SetType(Datum::DatumType::FLOAT);
					}
					else if (literal == "true" || literal == "false")
					{
						tempLiteral.SetType(Datum::DatumType::BOOLEAN);
					}
					else
					{
						tempLiteral.SetType(Datum::DatumType::INTEGER);
					}

					tempLiteral.SetFromString(literal, 0);
					mPostfixExpression->PushBack(tempName);

					mTempVariableCounter++;
					prev = pos + 1;
					continue;
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

	void ActionExpression::EvaluateExpression(const World& world)
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
				CallableFunctions::Iterator itr = mDefinedFunctions.Find(postfixExpression.Front());
				std::uint32_t numParams = itr->second.NumParams;
				Vector<Datum*> functionParams(numParams);
				Stack<Datum*> parameterStack;

				bool isResultDatumAParam = false;
				while (numParams > 0)
				{
					parameterStack.Push(evaluationStack.Top());
					
					if (resultDatums.IsEmpty() || evaluationStack.Top() != resultDatums.Top())
						isResultDatumAParam = true;
					else
						isResultDatumAParam = false;

					evaluationStack.Pop();
					numParams--;
				}

				while (!parameterStack.IsEmpty())
				{
					functionParams.PushBack(parameterStack.Top());
					parameterStack.Pop();
				}

				if (isResultDatumAParam)
					resultDatums.Push(new Datum());
				*resultDatums.Top() = itr->second.FunctionBody(functionParams);
				evaluationStack.Push(resultDatums.Top());
				postfixExpression.PopFront();
			}
			else
			{
				Datum* operand = nullptr;
				if (postfixExpression.Front().find('.')  < postfixExpression.Front().length())
					operand = world.ComplexSearch(postfixExpression.Front(), *this);
				else
					operand = Search(postfixExpression.Front());
				assert(operand != nullptr);
				if (operand->Type() == Datum::DatumType::REFERENCE)
					operand = &operand->Get<Datum>();
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
			switch (lhs.Type())
			{
			case Datum::DatumType::INTEGER:
				if(rhs.Type() == Datum::DatumType::INTEGER)
					lhs.Set(rhs.Get<std::int32_t>());
				else if (rhs.Type() == Datum::DatumType::FLOAT)
					lhs.Set(static_cast<std::int32_t>(rhs.Get<std::float_t>()));
				break;
			case Datum::DatumType::FLOAT:
				if (rhs.Type() == Datum::DatumType::FLOAT)
					lhs.Set(rhs.Get<std::float_t>());
				else if (rhs.Type() == Datum::DatumType::INTEGER)
					lhs.Set(static_cast<std::float_t>(rhs.Get<std::int32_t>()));
				break;
			case Datum::DatumType::STRING:
				lhs.Set(rhs.Get<std::string>());
				break;
			case Datum::DatumType::VECTOR4:
				lhs.Set(rhs.Get<glm::vec4>());
				break;
			case Datum::DatumType::MATRIX4x4:
				lhs.Set(rhs.Get<glm::mat4x4>());
				break;
			case Datum::DatumType::BOOLEAN:
				lhs.Set(rhs.Get<bool>());
				break;
			default:
				std::stringstream str;
				str << "Invalid operation. Cannot perform assignment on external datum of type " << Datum::DatumTypeToString[lhs.Type()];
				throw std::exception(str.str().c_str());
			}
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
