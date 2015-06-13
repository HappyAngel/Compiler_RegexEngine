#include <assert.h>
#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <stdlib.h>
#include "interfaces.h"
#include "RegexExpressionUnitTest.h"

using namespace std;

int main()
{
	//RegexExpressionUnitTest::RegexParserNFAUnitTest();	
	//RegexExpressionUnitTest::RegexParserDFAUnitTest();

	ifstream inputfile("test.txt");
	string line;
	string wholeString;

	if (inputfile.is_open())
	{
		while (getline(inputfile, line))
		{
			wholeString += line;
		}
	}

	RegexExpression regexExp("Pa*l");
	vector<string> results = regexExp.ExtractUsingDFA(wholeString);

	for (unsigned int i = 0; i < results.size(); i++)
	{
		cout << results[i] << endl;
	}


	int a;
	cin>>a;

	return 0;
}