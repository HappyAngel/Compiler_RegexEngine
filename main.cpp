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

	RegexExpression regexExp301("t.e");
	vector<string> results = regexExp301.ExtractUsingDFA("Paul Allen and Bill Gates, childhood friends with a passion for computer programming, sought to make a successful business utilizing their shared skills.[citation needed] In 1972 they founded their first company, named Traf-O-Data, which offered a rudimentary computer that tracked and analyzed automobile traffic data. Allen went on to pursue a degree in computer science at the University of Washington, later dropping out of school to work at Honeywell. Gates began studies at Harvard.[11] The January 1975 issue of Popular Electronics featured Micro Instrumentation and Telemetry Systems's (MITS) Altair 8800 microcomputer. Allen suggested that they could program a BASIC interpreter for the device; after a call from Gates claiming to have a working interpreter, MITS requested a demonstration. Since they didn't actually have one, Allen worked on a simulator for the Altair while Gates developed the interpreter. Although they developed the interpreter on a simulator and not the actual device, the interpreter worked flawlessly when they demonstrated the interpreter to MITS in Albuquerque, New Mexico in March 1975; MITS agreed to distribute it, marketing it as Altair BASIC.[9]:108, 112¨C114 They officially established Microsoft on April 4, 1975, with Gates as the CEO.[12] Allen came up with the original name of \"Micro - Soft, \" the combination of the words microprocessor and software, as recounted in a 1995 Fortune magazine article.[13][14] In August 1977 the company formed an agreement with ASCII Magazine in Japan, resulting in its first international office, \"ASCII Microsoft\".[15] The company moved to a new home in Bellevue, Washington in January 1979.[12]");
	for (unsigned int i = 0; i < results.size(); i++)
	{
		cout << results[i] << endl;
	}

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