#include <assert.h>
#include <iostream>
#include <stack>
#include <string>
#include <stdlib.h>
#include "interfaces.h"

using namespace std;


int GetValue(int op1, int op2, char c)
{
	if (c == '*')
	{
		return op1 * op2;
	}
	else if (c == '+')
	{
		return op1 + op2;
	}
	else
	{
		return -1;
	}
}

double GetOperatorPriority(char c)
{
	if (c == '*')
	{
		return 0.9;
	}
	else if (c == '+')
	{
		return 0.5;
	}
	// always have as the last char
	else if (c == '#') 
	{
		return 0;
	}
	else
	{
		return 0;
	}
}

bool isOperand(char c)
{
	if(!isdigit(c))
	{
		return false;
	}

	return true;
}

bool isOperator(char c)
{
	if (c == '+' || c =='*'|| c =='#' || c =='(' || c == ')')
	{
		return true;
	}
	else
	{
		return false;
	}
}

///preproess to parse for regex string
// 1 add "&" for AND regex operation
string preprocess(string str)
{
	string processedStr;

	if(str.empty())
	{
		return processedStr;
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

	return processedStr;
}

bool evalByOperatorType(RegexOperator* pRegOperator, stack<RegexOperand*>& operands)
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
bool eval(const string str, RegexExpression* re)
{
	string processedStr = preprocess(str);

	stack<RegexOperator*> operators;
	stack<RegexOperand*> operands;

	for (unsigned int i=0; i < processedStr.size(); i++)
	{
		if (RegexOperand::IsValid(processedStr[i]))
		{
			operands.push(new RegexOperand(processedStr[i]));
		}

		RegexOperator* pRegexOperator = NULL;

		if (NULL != (pRegexOperator = RegexOperatorFactory::GetRegexObject(processedStr[i])))
		{
			if (pRegexOperator->GetType() == RegexLeftParenthesesType)
			{
				operators.push(pRegexOperator);
			}
			else if (pRegexOperator->GetType() == RegexRightParenthesesType)
			{
				RegexOperator* pTmpRegexOperator = operators.empty() ? NULL : operators.top();
				
				if (NULL == pTmpRegexOperator)
				{
					return false;
				}

				while (pTmpRegexOperator->GetType() != RegexLeftParenthesesType)
				{
					if (!evalByOperatorType(pTmpRegexOperator, operands))
					{
						return false;
					}

					operators.pop();
					delete pTmpRegexOperator;
					
					if (operators.empty())
					{
						return false;
					}
					else
					{
						pTmpRegexOperator = operators.top();
					}
				}

				// assert GetType() == RegexLeftParenthesesType
				delete pTmpRegexOperator;
				operators.pop();
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

	}

	if ((!operators.empty() && ((RegexOperator*)operators.top())->GetType() != RegexEndType) 
		|| operands.size() != 1)
	{
		return false;
	}

	return true;
}

void RegexParserUnitTest()
{
	// positive cases
	string strRegex("a(b|c)*b"); 
	RegexExpression* pRegexExp = NULL;
	bool bSuccess1 = eval(strRegex, pRegexExp);
	assert(bSuccess1);

	string strRegex1("abc*b"); 
	pRegexExp = NULL;
	bSuccess1 = eval(strRegex1, pRegexExp);
	assert(bSuccess1);

	string strRegex2("a|b|c*b"); 
	pRegexExp = NULL;
	bSuccess1 = eval(strRegex2, pRegexExp);
	assert(bSuccess1);

	string strRegex3("a*b*c*|b"); 
	pRegexExp = NULL;
	bSuccess1 = eval(strRegex3, pRegexExp);
	assert(bSuccess1);

	// negative cases
	string strRegex4("|abc*b"); 
	pRegexExp = NULL;
	bSuccess1 = eval(strRegex4, pRegexExp);
	assert(!bSuccess1);

	string strRegex5("*abc*b"); 
	pRegexExp = NULL;
	bSuccess1 = eval(strRegex5, pRegexExp);
	assert(!bSuccess1);

	string strRegex6("|a"); 
	pRegexExp = NULL;
	bSuccess1 = eval(strRegex6, pRegexExp);
	assert(!bSuccess1);

	string strRegex7("(a"); 
	pRegexExp = NULL;
	bSuccess1 = eval(strRegex7, pRegexExp);
	assert(!bSuccess1);

	string strRegex8("aa)"); 
	pRegexExp = NULL;
	bSuccess1 = eval(strRegex8, pRegexExp);
	assert(!bSuccess1);

	string strRegex9("a)"); 
	pRegexExp = NULL;
	bSuccess1 = eval(strRegex9, pRegexExp);
	assert(!bSuccess1);

	cout<<"ALL RegexParser Unit Test Passed!"<<endl;
}

int main()
{
	RegexParserUnitTest();


	//cout<<"Convert string to Regex Parse tree done with result: "<< (bSuccess1 ? "True" : "False") <<endl;
	int a;
	cin>>a;

	return 0;
}