#include "interfaces.h"

bool FA::evalByOperatorType(RegexOperator* pRegOperator, stack<RegexOperand*>& operands)
{
	if (pRegOperator == NULL || operands.empty())
	{
		return false;
	}

	vector<RegexOperand*> pVec;
	RegexOperand* pOp1 = NULL;
	RegexOperand* pOp2 = NULL;

	switch (pRegOperator->GetType())
	{
	case RegexAndOperatorType:
	case RegexOrOperatorType:

		if (operands.size() < 2)
		{
			return false;
		}

		pOp1 = operands.top();
		operands.pop();
		pOp2 = operands.top();
		operands.pop();
		pVec.push_back(pOp1);
		pVec.push_back(pOp2);
		operands.push((RegexOperand*)pRegOperator->eval(this, pVec));
		delete pOp1;
		delete pOp2;
		break;

	case RegexStarOperatorType:
		pOp1 = operands.top();
		operands.pop();
		pVec.push_back(pOp1);
		operands.push((RegexOperand*)pRegOperator->eval(this, pVec));
		delete pOp1;
		break;
	default:
		return false;
	}

	return true;
}

// eval str value
// return true on success and set outputValue
bool FA::eval()
{
	string processedStr = _normalizedExp;
	stack<RegexOperator*> operators;
	stack<RegexOperand*> operands;

	for (unsigned int i = 0; i < processedStr.size(); i++)
	{
		if (isSupportSymbol(processedStr[i]))
		{
			operands.push(new RegexOperand(createSingleCharState(processedStr[i])));
		}
		else
		{
			RegexOperator* pRegexOperator = RegexOperatorFactory::GetRegexObject(processedStr[i]);

			if (NULL != pRegexOperator)
			{
				if (pRegexOperator->GetType() == RegexLeftParenthesesType)
				{
					operators.push(pRegexOperator);
				}
				else if (pRegexOperator->GetType() == RegexRightParenthesesType)
				{
					if (operators.empty())
					{
						return false;
					}

					RegexOperator* pTmpRegexOperator = operators.top();
					operators.pop();

					while (pTmpRegexOperator->GetType() != RegexLeftParenthesesType)
					{
						if (!evalByOperatorType(pTmpRegexOperator, operands))
						{
							return false;
						}

						delete pTmpRegexOperator;

						if (operators.empty())
						{
							return false;
						}
						else
						{
							pTmpRegexOperator = operators.top();
							operators.pop();
						}
					}

					// assert GetType() == RegexLeftParenthesesType
					delete pTmpRegexOperator;
				}
				else
				{
					RegexOperator* pTmpRegexOperator = NULL;

					while (!operators.empty() && (pTmpRegexOperator = operators.top())->GetPriority() >= pRegexOperator->GetPriority())
					{
						if (!evalByOperatorType(pTmpRegexOperator, operands))
						{
							return false;
						}

						operators.pop();
						delete pTmpRegexOperator;
					}

					operators.push(pRegexOperator);
				}
			}
			else
			{
				return false;
			}
		}

	}

	if ((!operators.empty() && ((RegexOperator*)operators.top())->GetType() != RegexEndType)
		|| operands.size() != 1)
	{
		return false;
	}

	RegexOperand* rgOperand = operands.top();
	this->_startState = rgOperand->GetValue().startState;
	this->_endState.push_back(rgOperand->GetValue().endState);
	delete rgOperand;
	operands.pop();

	return true;
}

FA::State FA::createState()
{
	State newState = _allState.size();
	_allState.push_back(newState);
	return newState;
}