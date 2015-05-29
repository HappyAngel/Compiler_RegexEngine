#include "interfaces.h"

bool NFA::IsAccept(string strToParse)
{
	if (_transitionTable.empty())
	{
		return false;
	}

	vector<State> currentStates;
	currentStates.push_back(_startState);
	vector<State> tmpStates;
	// use simulate NFA algorithms to get results
	for (unsigned int i = 0; i < strToParse.length(); i++)
	{
		currentStates = findALLNULLPathStatesFromStates(currentStates);

		for (unsigned int j = 0; j < currentStates.size(); j++)
		{
			if (_transitionTable.size() > currentStates[j] && _transitionTable[currentStates[j]].find(strToParse[i]) != _transitionTable[currentStates[j]].end())
			{
				tmpStates.insert(tmpStates.end(), _transitionTable[currentStates[j]][strToParse[i]].begin(), _transitionTable[currentStates[j]][strToParse[i]].end());
			}
		}

		if (tmpStates.empty())
		{
			return false;
		}
		else
		{
			currentStates = tmpStates;
			tmpStates.clear();
		}
	}

	currentStates = findALLNULLPathStatesFromStates(currentStates);

	if (containsEndStates(currentStates))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool NFA::containsEndStates(vector<State> states)
{
	for (unsigned int i = 0; i < states.size(); i++)
	{
		for (unsigned int j = 0; j < _endState.size(); j++)
		{
			if (states[i] == _endState[j])
			{
				return true;
			}
		}
	}

	return false;
}

vector<NFA::State> NFA::findALLNULLPathStatesFromStates(vector<State> states)
{
	vector<State> results = states;

	unsigned int headIndex = 0;
	unsigned int tailIndex = results.size();

	while (headIndex < tailIndex)
	{
		if (_transitionTable.size() > results[headIndex] &&
			_transitionTable[results[headIndex]].find(kNULLTransite) != _transitionTable[results[headIndex]].end())
		{
			// to do: may add the same states.
			results.insert(results.end(), _transitionTable[results[headIndex]][kNULLTransite].begin(), _transitionTable[results[headIndex]][kNULLTransite].end());
		}

		headIndex++;
		tailIndex = results.size();
	}

	return results;
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
	while (startState >= _transitionTable.size())
	{
		map<char, vector<State>> tmpMap;
		_transitionTable.push_back(tmpMap);
	}

	if (_transitionTable[startState].find(c) != _transitionTable[startState].end())
	{
		_transitionTable[startState][c].push_back(endState);
	}
	else
	{
		vector<State> endStates;
		endStates.push_back(endState);
		_transitionTable[startState].insert(std::pair<char, vector<State>>(c, endStates));
	}

	return true;
}

NFA::SubNFAGraphInfo NFA::performANDOperator(SubNFAGraphInfo dest, SubNFAGraphInfo src)
{
	createTransite(src.endState, kNULLTransite, dest.startState);

	SubNFAGraphInfo info;
	info.startState = src.startState;
	info.endState = dest.endState;
	return info;
}

NFA::SubNFAGraphInfo NFA::performOROperator(SubNFAGraphInfo dest, SubNFAGraphInfo src)
{
	State startState = createState();
	createTransite(startState, kNULLTransite, dest.startState);
	createTransite(startState, kNULLTransite, src.startState);

	State endState = createState();
	createTransite(src.endState, kNULLTransite, endState);
	createTransite(dest.endState, kNULLTransite, endState);

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