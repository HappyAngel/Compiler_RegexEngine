#include "RegexExpressionUnitTest.h"
#include <assert.h>

void RegexExpressionUnitTest::RegexParserUnitTest()
{
	// positive cases
	RegexExpression regexExp("a(b|c)*b"); 
	bool bSuccess1 = regexExp.ParseUsingNFA("test");
	assert(bSuccess1);

	RegexExpression regexExp1("abc*b"); 
	bSuccess1 = regexExp1.ParseUsingNFA("test");
	assert(bSuccess1);

	RegexExpression regexExp2("a|b|c*b"); 
	bSuccess1 = regexExp2.ParseUsingNFA("test");
	assert(bSuccess1);

	RegexExpression regexExp3("a*b*c*|b"); 
	bSuccess1 = regexExp3.ParseUsingNFA("test");
	assert(bSuccess1);

	RegexExpression regexExp100("a*"); 
	bSuccess1 = regexExp100.ParseUsingNFA("test");
	assert(bSuccess1);

	RegexExpression regexExp101("a|b"); 
	bSuccess1 = regexExp101.ParseUsingNFA("test");
	assert(bSuccess1);

	RegexExpression regexExp102("ab"); 
	bSuccess1 = regexExp102.ParseUsingNFA("test");
	assert(bSuccess1);

	RegexExpression regexExp103("(a*)"); 
	bSuccess1 = regexExp103.ParseUsingNFA("test");
	assert(bSuccess1);

	// negative cases
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