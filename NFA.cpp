#include "interfaces.h"

bool NFA::isAccept(string strToParse)
{
	int index = simulateNFA(strToParse);
	
	if (index < 0 || (!strToParse.empty() && index == 0))
	{
		return false;
	}
	else
	{
		return true;
	}
}

// parse a string {strToParse} and return the accept string [0, index) in the string
// if no accept occues, return -1
int NFA::simulateNFA(string strToParse)
{
	if (_transitionTable.empty())
	{
		return -1;
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
			return i;
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
		return strToParse.length();
	}
	else
	{
		return -1;
	}
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

bool NFA::buildNFA()
{
	if (_normalizedExp.empty())
	{
		return false;
	}

	return eval();
}

int NFA::FindFirstMatchedString(string strToExtract)
{
	return simulateNFA(strToExtract);
}