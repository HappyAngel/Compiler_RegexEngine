#include <iostream>
#include <stack>
#include <string>
#include <stdlib.h>


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

// eval str value
// return true on success and set outputValue
bool eval(string str, int& outputValue)
{
	stack<char> operators;
	stack<int> operands;
	str.append("#");

	for (int i=0; i < str.size(); i++)
	{
		if (isOperand(str[i]))
		{
			char c = str[i];
			operands.push(atoi(&c));
		}

		if (isOperator(str[i]))
		{
			if (str[i] == '(')
			{
				operators.push(str[i]);
			}
			else if (str[i] == ')')
			{
				while (!operators.empty())
				{
					if (operators.top() == '(')
					{
						operators.pop();
						break;
					}
					else
					{
						if (operands.size() > 2)
						{
							int op1 = operands.top();
							operands.pop();
							int op2 = operands.top();
							operands.pop();

							operands.push(GetValue(op1,op2, operators.top()));
							operators.pop();
						}
						else
						{
							return false;
						}
					}
				}
			}
			else
			{
				while (!operators.empty() && GetOperatorPriority(operators.top()) >= GetOperatorPriority(str[i]))
				{
					if (operands.size() >= 2)
					{
						int op1 = operands.top();
						operands.pop();
						int op2 = operands.top();
						operands.pop();

						operands.push(GetValue(op1,op2, operators.top()));
						operators.pop();
					}
					else
					{
						return false;
					}
				}

				operators.push(str[i]);
			}
		}
	}

	if ((!operators.empty() && operators.top() != '#') || operands.size() != 1)
	{
		return false;
	}

	outputValue = operands.top();

	return true;
}



int main()
{
	string strRegex("2*(1+2)"); 
	int value = -1;

	bool bSuccess1 = eval(strRegex, value);

	cout<<value<<endl;

	int a;
	cin>>a;

	return 0;
}