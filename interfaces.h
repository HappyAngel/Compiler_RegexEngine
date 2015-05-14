#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

class IOperand
{
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
private:
	char _c;

public:
	RegexOperand()
	{
		_c = '#';
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

};