#pragma once

#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <stack>
using namespace std;

class IOperand
{
protected:
	char _c;
public:

	virtual char GetValue() = 0;
};

///
//  this class contains base classese for the Regex engine
//
//
///
class RegexOperand : public IOperand
{
public:
	RegexOperand()
	{
		_c = '_';
	}

	RegexOperand (char c)
	{
		if (!IsValid(c))
		{
			throw std::invalid_argument("we only support a-z and A-Z now");
		}

		_c = c;
	}

	bool static IsValid (char c)
	{
		if((c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z'))
		{
			return true;
		}

		return false;
	}

	virtual char GetValue()
	{
		return _c;
	}

	static bool CheckAndCreateOperand(char c, RegexOperand* pOperand)
	{
		if (IsValid(c))
		{
			pOperand = new RegexOperand(c);
		}
	}
};

class IOperator
{
public:
	// get priority in number of the current operator
	// to help compare 
	virtual double GetPriority() = 0;

	virtual IOperand* eval(vector<IOperand*> opVector) = 0;
}; 


enum RegexOperatorType
{
	UnknownRegexOperatorType=0,
	RegexStarOperatorType,
	RegexOrOperatorType,
	RegexAndOperatorType,
	RegexLeftParenthesesType,
	RegexRightParenthesesType,
	RegexEndType
};

class RegexOperator : public IOperator
{
public:
	virtual RegexOperatorType GetType() = 0;
};

class RegexStarOperator : public RegexOperator
{
public:
	virtual IOperand* eval(vector<IOperand*> opVector)
	{
		cout<<"call RegexStarOperator eval() to operate on "<<opVector[0]->GetValue()<<endl;
		return new RegexOperand();
	}

	virtual double GetPriority()
	{
		return 0.9;
	}

	virtual RegexOperatorType GetType()
	{
		return RegexOperatorType::RegexStarOperatorType;
	}
};

class RegexOrOperator : public RegexOperator
{
public:
	virtual IOperand* eval(vector<IOperand*> opVector)
	{
		cout<<"call RegexOrOperator eval() on "<<opVector[0]->GetValue()<<" and "<<opVector[1]->GetValue()<<endl;
		return new RegexOperand();
	}

	virtual double GetPriority()
	{
		return 0.2;
	}

	virtual RegexOperatorType GetType()
	{
		return RegexOperatorType::RegexOrOperatorType;
	}
};

class RegexAndOperator : public RegexOperator
{
public:
	virtual IOperand* eval(vector<IOperand*> opVector)
	{
		cout<<"call RegexAndOperator eval()"<<opVector[0]->GetValue()<<" and "<<opVector[1]->GetValue()<<endl;
		return new RegexOperand();
	}	

	virtual double GetPriority()
	{
		return 0.5;
	}

	virtual RegexOperatorType GetType()
	{
		return RegexOperatorType::RegexAndOperatorType;
	}
};

class RegexLeftParentheseOperator : public RegexOperator
{
public:
	virtual IOperand* eval(vector<IOperand*> opVector)
	{
		cout<<"call RegexLeftParentheseOperator eval()"<<opVector.size()<<endl;
		// do nothing

		return new RegexOperand();
	}	

	virtual double GetPriority()
	{
		return 0;
	}

	virtual RegexOperatorType GetType()
	{
		return RegexOperatorType::RegexLeftParenthesesType;
	}
};

class RegexRightParentheseOperator : public RegexOperator
{
public:
	virtual IOperand* eval(vector<IOperand*> opVector)
	{
		cout<<"call RegexRightParentheseOperator eval()"<<opVector.size()<<endl;
		// do nothing
		return new RegexOperand();
	}	

	virtual double GetPriority()
	{
		return 0;
	}

	virtual RegexOperatorType GetType()
	{
		return RegexOperatorType::RegexRightParenthesesType;
	}
};

class RegexEndOperator : public RegexOperator
{
public:
	virtual IOperand* eval(vector<IOperand*> opVector)
	{
		cout<<"call RegexEndOperator eval()"<<opVector.size()<<endl;
		// do nothing
		return new RegexOperand();
	}	

	virtual double GetPriority()
	{
		return 0;
	}

	virtual RegexOperatorType GetType()
	{
		return RegexOperatorType::RegexEndType;
	}
};

class RegexOperatorFactory
{
private:
	RegexOperatorFactory()
	{
	}

public:
	///error returns NULL, otherwise the regex object
	static RegexOperator* GetRegexObject(char c)
	{
		RegexOperator* returnObj = NULL;

		switch (c)
		{
		case '*':
			returnObj =  new RegexStarOperator();
			break;
		case '|':
			returnObj = new RegexOrOperator();
			break;
		case '&':
			returnObj = new RegexAndOperator();
			break;
		case '(':
			returnObj = new RegexLeftParentheseOperator();
			break;
		case ')':
			returnObj = new RegexRightParentheseOperator();
			break;
		case '#':
			returnObj = new RegexEndOperator();
			break;
		default:
			return returnObj;
		}

		return returnObj;
	}

};

class RegexExpression
{
private:
	// raw regex expression to parse
	string _exp;
	// exp after preprocessor
	string _normalizedExp;

	///preproess to parse for regex string
	// 1 add "&" for AND regex operation
	// 2 add "#" for Regex End operation
	void preprocess();

	bool evalByOperatorType(RegexOperator* pRegOperator, stack<RegexOperand*>& operands);

	// eval str value
	// return true on success and set outputValue
	bool eval();

public:
	RegexExpression(string exp)
	{
		_exp = exp;

		preprocess();
	}

	// parse exp to get NFA, and simulate to get results
	bool ParseUsingNFA(string strToParse)
	{
		return eval();
	}

	// parse exp using DFA
	bool ParseUsingDFA(string strToParse)
	{

	}
};

// base class for finite automa machine
class FA
{
protected:
	typedef unsigned int State;

	State _startState;
	vector<State> _endState;
	vector<State> _allState;
	vector<vector<char>> _transitionTable;
private:
	virtual bool isSupportSymbol (char c) = 0;

	virtual bool Transite (State s, char c) = 0;

public:
	FA(RegexExpression re);
};