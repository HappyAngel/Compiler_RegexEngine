#pragma once

#include "interfaces.h"

class NFA : public FA
{
public:
	NFA(RegexExpression re) : FA(re)
	{
	}

	virtual ~NFA(void)
	{

	}

};

