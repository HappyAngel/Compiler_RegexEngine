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
	RegexExpressionUnitTest::RegexParserNFAUnitTest();	
	RegexExpressionUnitTest::RegexParserDFAUnitTest();

	//RegexExpression regexExp301("P.?a");
	//vector<string> results = regexExp301.ExtractUsingDFA("Piass");
	//for (unsigned int i = 0; i < results.size(); i++)
	//{
	//	cout << results[i] << endl;
	//}

	/*ifstream inputfile("test.txt");
	string line;
	string wholeString;

	if (inputfile.is_open())
	{
		while (getline(inputfile, line))
		{
			wholeString += line;
		}
	}

	RegexExpression regexExp("a.*c");
	vector<string> results = regexExp.ExtractUsingDFA(wholeString);

	for (unsigned int i = 0; i < results.size(); i++)
	{
		cout << results[i] << endl;
	}

	cout << results.size() << endl;*/

	int a;
	cin>>a;

	return 0;
}