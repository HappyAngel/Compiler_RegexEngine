#pragma once

#include "interfaces.h"

class NFA : public FA
{
private:
	bool isSupportSymbol (char c);

	bool Transite (State s, char c);

public:
	NFA(RegexExpression re) : FA(re)
	{
	}

	virtual ~NFA(void)
	{

	}

};

