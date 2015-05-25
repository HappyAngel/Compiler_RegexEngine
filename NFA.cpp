#include "interfaces.h"

bool NFA::IsAccept(string strToParse)
{
	if (_transitionTable.empty())
	{
		return false;
	}

	return true;
}

bool NFA::BuildNFA()
{
	if (_normalizedExp.empty())
	{
		return false;
	}

	return eval();
}

bool NFA::isSupportSymbol(char c)
{
	if ((c >= 'a' && c <= 'z')
		|| c >= 'A' && c <= 'Z')
	{
		return true;
	}

	return false;
}

bool NFA::createTransite(State startState, char c, State endState)
{
	while (startState > _transitionTable.size())
	{
		map<char, vector<State>> tmpMap;
		_transitionTable.push_back(tmpMap);
	}

	if (_transitionTable[startState - 1].find(c) != _transitionTable[startState - 1].end())
	{
		_transitionTable[startState - 1][c].push_back(endState);
	}
	else
	{
		vector<State> endStates;
		endStates.push_back(endState);
		_transitionTable[startState - 1].insert(std::pair<char, vector<State>>(c, endStates));
	}

	return true;
}

NFA::SubNFAGraphInfo NFA::performANDOperator(SubNFAGraphInfo a, SubNFAGraphInfo b)
{
	createTransite(a.endState, kNULLTransite, b.startState);

	SubNFAGraphInfo info;
	info.startState = a.startState;
	info.endState = b.endState;
	return info;
}

NFA::SubNFAGraphInfo NFA::performOROperator(SubNFAGraphInfo a, SubNFAGraphInfo b)
{
	State startState = createState();
	createTransite(startState, kNULLTransite, a.startState);
	createTransite(startState, kNULLTransite, b.startState);

	State endState = createState();
	createTransite(a.endState, kNULLTransite, endState);
	createTransite(b.endState, kNULLTransite, endState);

	SubNFAGraphInfo info;
	info.startState = startState;
	info.endState = endState;

	return info;
}

NFA::SubNFAGraphInfo NFA::performSTAROperator(SubNFAGraphInfo a)
{
	createTransite(a.endState, kNULLTransite, a.startState);

	State startState = createState();
	State endState = createState();

	createTransite(startState, kNULLTransite, a.startState);
	createTransite(startState, kNULLTransite, endState);
	createTransite(a.endState, kNULLTransite, endState);

	SubNFAGraphInfo info;
	info.startState = startState;
	info.endState = endState;
	return info;
}

NFA::SubNFAGraphInfo NFA::createSingleCharState(char a)
{
	State startState = createState();
	State endState = createState();
	createTransite(startState, a, endState);

	NFA::SubNFAGraphInfo info;
	info.startState = startState;
	info.endState = endState;

	return info;
}

///preproess to parse for regex string
// 1 add "&" for AND regex operation
// 2 add "#" for Regex End operation
void NFA::preprocess()
{
	string processedStr;
	string str = _exp;

	if (str.empty())
	{
		return;
	}

	// step1: add operator '&'
	int indexP = -1;

	for (unsigned int i = 0; i < str.size(); i++)
	{
		if (indexP != -1 && (
			((isSupportSymbol(str[indexP])
			|| RegexOperatorFactory::GetRegexObject(str[indexP])->GetType() == RegexStarOperatorType
			|| RegexOperatorFactory::GetRegexObject(str[indexP])->GetType() == RegexRightParenthesesType)
			&& isSupportSymbol(str[i]))
			|| (isSupportSymbol(str[indexP]) && RegexOperatorFactory::GetRegexObject(str[i])->GetType() == RegexLeftParenthesesType)))
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