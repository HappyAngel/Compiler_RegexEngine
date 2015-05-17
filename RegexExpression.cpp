/// definition of Class RegexExpression

#include "interfaces.h"

///preproess to parse for regex string
// 1 add "&" for AND regex operation
// 2 add "#" for Regex End operation
void RegexExpression::preprocess()
{
	string processedStr;
	string str = _exp;

	if(str.empty())
	{
		return;
	}

	// step1: add operator '&'
	int indexP = -1;

	for (unsigned int i=0; i < str.size(); i++)
	{
		if (indexP != -1 && (
			((RegexOperand::IsValid(str[indexP]) 
			|| RegexOperatorFactory::GetRegexObject(str[indexP])->GetType() == RegexStarOperatorType
			|| RegexOperatorFactory::GetRegexObject(str[indexP])->GetType() == RegexRightParenthesesType)
			&& RegexOperand::IsValid(str[i]))
			|| (RegexOperand::IsValid(str[indexP]) && RegexOperatorFactory::GetRegexObject(str[i])->GetType() == RegexLeftParenthesesType)))
		{
			processedStr += '&';
		}

		processedStr += str[i];
		indexP = i;
	}

	//step2: add '#' as sentinel
	processedStr += '#';
	
	_normalizedExp = processedStr;
}

bool RegexExpression::evalByOperatorType(RegexOperator* pRegOperator, stack<RegexOperand*>& operands)
{
	if (pRegOperator == NULL || operands.empty())
	{
		return false;
	}

	vector<IOperand*> pVec;
	IOperand* pOp1 = NULL;
	IOperand* pOp2 = NULL;

	switch (pRegOperator->GetType())
	{
	case RegexAndOperatorType:
	case RegexOrOperatorType:
		{
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
			operands.push((RegexOperand*)pRegOperator->eval(pVec));
			delete pOp1;
			delete pOp2;
			break;
		}
	case RegexStarOperatorType:
		pOp1 = operands.top();
		operands.pop();
		pVec.push_back(pOp1);
		operands.push((RegexOperand*)pRegOperator->eval(pVec));
		delete pOp1;
		break;
	default:
		return false;
	}

	return true;
}

// eval str value
// return true on success and set outputValue
bool RegexExpression::eval()
{
	string processedStr = _normalizedExp;
	stack<RegexOperator*> operators;
	stack<RegexOperand*> operands;

	for (unsigned int i=0; i < processedStr.size(); i++)
	{
		if (RegexOperand::IsValid(processedStr[i]))
		{
			operands.push(new RegexOperand(processedStr[i]));
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

	return true;
}