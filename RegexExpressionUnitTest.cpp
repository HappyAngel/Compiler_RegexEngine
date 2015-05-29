#include "RegexExpressionUnitTest.h"
#include <assert.h>

void RegexExpressionUnitTest::RegexParserUnitTest()
{
	// positive cases
	RegexExpression regexExp("a(b|c)*b"); 
	bool bSuccess1 = regexExp.ParseUsingNFA("ab");
	assert(bSuccess1);

	RegexExpression regexExp1("abc*b"); 
	bSuccess1 = regexExp1.ParseUsingNFA("abcccb");
	assert(bSuccess1);

	RegexExpression regexExp2("a|b|c*b"); 
	bSuccess1 = regexExp2.ParseUsingNFA("b");
	assert(bSuccess1);

	RegexExpression regexExp3("a*b*c*|b"); 
	bSuccess1 = regexExp3.ParseUsingNFA("ab");
	assert(bSuccess1);

	RegexExpression regexExp100("a*"); 
	bSuccess1 = regexExp100.ParseUsingNFA("");
	assert(bSuccess1);

	RegexExpression regexExp101("a|b"); 
	bSuccess1 = regexExp101.ParseUsingNFA("b");
	assert(bSuccess1);

	RegexExpression regexExp102("ab"); 
	bSuccess1 = regexExp102.ParseUsingNFA("ab");
	assert(bSuccess1);

	RegexExpression regexExp103("(a*)"); 
	bSuccess1 = regexExp103.ParseUsingNFA("a");
	assert(bSuccess1);

	RegexExpression regexExp104("(a|cb)e*f"); 
	bSuccess1 = regexExp104.ParseUsingNFA("cbef");
	assert(bSuccess1);

	// negative cases: not match
	RegexExpression regexExp200("abc");
	bSuccess1 = regexExp200.ParseUsingNFA("ab");
	assert(!bSuccess1);

	RegexExpression regexExp201("a|b|c");
	bSuccess1 = regexExp201.ParseUsingNFA("ddd");
	assert(!bSuccess1);

	RegexExpression regexExp202("a*b");
	bSuccess1 = regexExp202.ParseUsingNFA("c");
	assert(!bSuccess1);

	RegexExpression regexExp203("ab|c*");
	bSuccess1 = regexExp203.ParseUsingNFA("vv");
	assert(!bSuccess1);

	// negative cases: wrong syntax
	RegexExpression regexExp4("|abc*b"); 
	bSuccess1 = regexExp4.ParseUsingNFA("test");
	assert(!bSuccess1);

	RegexExpression regexExp5("*abc*b"); 
	bSuccess1 = regexExp5.ParseUsingNFA("test");
	assert(!bSuccess1);

	RegexExpression regexExp6("|a"); 
	bSuccess1 = regexExp6.ParseUsingNFA("test");
	assert(!bSuccess1);

	RegexExpression regexExp7("(a"); 
	bSuccess1 = regexExp7.ParseUsingNFA("test");
	assert(!bSuccess1);

	RegexExpression regexExp8("aa)"); 
	bSuccess1 = regexExp8.ParseUsingNFA("test");
	assert(!bSuccess1);

	RegexExpression regexExp9("a)"); 
	bSuccess1 = regexExp9.ParseUsingNFA("test");
	assert(!bSuccess1);

	cout<<"ALL RegexParser Unit Test Passed!"<<endl;
}