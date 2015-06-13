#include "interfaces.h"

// parse a string {strToParse} and return the accept string's index, where [0, index) is the accept string
// if no accept occurs, return -1
int NFA::simulateNFA(string strToParse)
{
	if (_transitionTable.empty())
	{
		return -1;
	}

	vector<State> currentStates;
	currentStates.push_back(_startState);
	currentStates = findALLNULLPathStatesFromStates(currentStates);

	vector<State> tmpStates;

	// use simulate NFA algorithms to get results
	unsigned int i = 0;

	for (; i < strToParse.length(); i++)
	{
		for (unsigned int j = 0; j < currentStates.size(); j++)
		{
			if (_transitionTable.size() > currentStates[j] && _transitionTable[currentStates[j]].find(strToParse[i]) != _transitionTable[currentStates[j]].end())
			{
				tmpStates.insert(tmpStates.end(), _transitionTable[currentStates[j]][strToParse[i]].begin(), _transitionTable[currentStates[j]][strToParse[i]].end());
			}
		}

		if (tmpStates.empty())
		{
			break;
		}
		else
		{
			currentStates = tmpStates;
			currentStates = findALLNULLPathStatesFromStates(currentStates);
			tmpStates.clear();
		}
	}

	// stuck in some notes or complete the graph 
	if (containsEndStates(currentStates))
	{
		return i;
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