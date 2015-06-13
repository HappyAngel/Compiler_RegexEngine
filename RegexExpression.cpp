/// definition of Class RegexExpression

#include "interfaces.h"

bool RegexExpression::ParseUsingNFA(string strToParse)
{
	NFA nfa(_exp);

	if (!nfa.buildNFA())
	{
		return false;
	}

	return nfa.isAccept(strToParse);
}

bool RegexExpression::ParseUsingDFA(string strToParse)
{
	NFA nfa(_exp);

	if (!nfa.buildNFA())
	{
		return false;
	}

	DFA dfa(&nfa);

	if (!dfa.buildDFAFromNFA())
	{
		return false;
	}

	return dfa.isAccept(strToParse);
}

// extract strings using NFA
vector<string> RegexExpression::ExtractUsingNFA(string strToExtract)
{
	vector<string> result;

	NFA nfa(_exp);

	if (!nfa.buildNFA())
	{
		return result;
	}

	result = nfa.extractMatchStrings(strToExtract);
	return result;
}

vector<string> RegexExpression::ExtractUsingDFA(string strToExtract)
{
	vector<string> result;

	NFA nfa(_exp);

	if (!nfa.buildNFA())
	{
		return result;
	}

	DFA dfa(&nfa);

	if (!dfa.buildDFAFromNFA())
	{
		return result;
	}

	return dfa.extractMatchStrings(strToExtract);
}