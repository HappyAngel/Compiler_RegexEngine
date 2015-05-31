#pragma once

#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <stack>
#include <map>
using namespace std;

class RegexExpression;

// base class for finite automa machine
class FA
{
protected:
	typedef unsigned int State;
	typedef struct SubNFAGraphInfo
	{
		State startState;
		State endState;
	}SubNFAGraphInfo;

	State _startState;
	vector<State> _endState;
	vector<State> _allState;
	vector<map<char,vector<State>>> _transitionTable;

	// raw regex expression to parse
	string _exp;

	// exp after preprocessor
	string _normalizedExp;

	///
	//  this class contains base classese for the Regex engine
	//
	//
	///
	class RegexOperand
	{
	private:
		SubNFAGraphInfo _info;

	public:

		RegexOperand(SubNFAGraphInfo info)
		{
			_info = info;
		}


		SubNFAGraphInfo GetValue()
		{
			return _info;
		}
	};

	enum RegexOperatorType
	{
		UnknownRegexOperatorType = 0,
		RegexStarOperatorType,
		RegexOrOperatorType,
		RegexAndOperatorType,
		RegexLeftParenthesesType,
		RegexRightParenthesesType,
		RegexEndType
	};

	class RegexOperator
	{
	public:
		virtual RegexOperatorType GetType() = 0;

		// get priority in number of the current operator
		// to help compare 
		virtual double GetPriority() = 0;

		virtual RegexOperand* eval(FA* fa, vector<RegexOperand*> opVector) = 0;
	};

	class RegexStarOperator : public RegexOperator
	{
	public:
		virtual RegexOperand* eval(FA* fa, vector<RegexOperand*> opVector)
		{
			return new RegexOperand(fa->performSTAROperator(opVector[0]->GetValue()));
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
		virtual RegexOperand* eval(FA* fa, vector<RegexOperand*> opVector)
		{
			return new RegexOperand(fa->performOROperator(opVector[0]->GetValue(), opVector[1]->GetValue()));
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
		virtual RegexOperand* eval(FA* fa, vector<RegexOperand*> opVector)
		{
			return new RegexOperand(fa->performANDOperator(opVector[0]->GetValue(), opVector[1]->GetValue()));
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
		virtual RegexOperand* eval(FA* fa, vector<RegexOperand*> opVector)
		{
			SubNFAGraphInfo info;
			info.startState = 0;
			info.endState = 0;
			return new RegexOperand(info);
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
		virtual RegexOperand* eval(FA* fa, vector<RegexOperand*> opVector)
		{
			SubNFAGraphInfo info;
			info.startState = 0;
			info.endState = 0;
			return new RegexOperand(info);
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
		virtual RegexOperand* eval(FA* fa, vector<RegexOperand*> opVector)
		{
			SubNFAGraphInfo info;
			info.startState = 0;
			info.endState = 0;
			return new RegexOperand(info);
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
				returnObj = new RegexStarOperator();
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


protected:
	virtual bool isSupportSymbol (char c) = 0;

	virtual SubNFAGraphInfo createSingleCharState(char a) = 0;

	virtual bool createTransite(State startState, char c, State endState) = 0;

	State createState();

	///preproess to parse for regex string
	// 1 add "&" for AND regex operation
	// 2 add "#" for Regex End operation
	virtual void preprocess() = 0;

	virtual bool evalByOperatorType(RegexOperator* pRegOperator, stack<RegexOperand*>& operands);

	// eval str value
	// return true on success and set outputValue
	virtual bool eval();

	virtual SubNFAGraphInfo performANDOperator(SubNFAGraphInfo a, SubNFAGraphInfo b) = 0;

	virtual SubNFAGraphInfo performOROperator(SubNFAGraphInfo a, SubNFAGraphInfo b) = 0;

	virtual SubNFAGraphInfo performSTAROperator(SubNFAGraphInfo a) = 0;

public:
	FA(string regexStr)
	{
		_exp = regexStr;
	}

	virtual bool IsAccept(string strToParse) = 0;

	virtual vector<string> extractMatchStrings(string strToExtract) = 0;
};

class NFA : public FA
{
private:
	virtual bool isSupportSymbol(char c);

	virtual void preprocess();

	virtual bool createTransite(State startState, char c, State endState);

	virtual SubNFAGraphInfo performANDOperator(SubNFAGraphInfo a, SubNFAGraphInfo b);

	virtual SubNFAGraphInfo performOROperator(SubNFAGraphInfo a, SubNFAGraphInfo b);

	virtual SubNFAGraphInfo performSTAROperator(SubNFAGraphInfo a);

	virtual SubNFAGraphInfo createSingleCharState(char a);

	vector<State> findALLNULLPathStatesFromStates(vector<State> states);

	bool containsEndStates(vector<State> states);

	const char kNULLTransite = '~';

	int simulateNFA(string strToParse);

public:
	NFA(string regexStr) : FA(regexStr)
	{
		preprocess();
	}

	bool BuildNFA();

	virtual bool IsAccept(string strToParse);

	virtual vector<string> extractMatchStrings(string strToExtract);
};

class RegexExpression
{
private:
	// raw regex expression to parse
	string _exp;


public:
	RegexExpression(string exp)
	{
		_exp = exp;

	}

	string GetRegexString()
	{
		return _exp;
	}

	// parse exp to get NFA, and simulate to get results
	bool ParseUsingNFA(string strToParse)
	{
		NFA nfa(_exp);

		if (!nfa.BuildNFA())
		{
			return false;
		}

		if (!nfa.IsAccept(strToParse))
		{
			return false;
		}
		
		return true;
	}

	// parse exp using DFA
	bool ParseUsingDFA(string strToParse)
	{

	}
	
	// extract strings using NFA
	vector<string> ExtractUsingNFA(string strToExtract)
	{
		vector<string> result;

		NFA nfa(_exp);

		if (!nfa.BuildNFA())
		{
			return result;
		}

		result = nfa.extractMatchStrings(strToExtract);
		return result;
	}
};