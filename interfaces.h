__interface IOperator
{
	// get priority in number of the current operator
	// to help compare 
	double GetPriority();

}; 

__interface IOperand
{
	char GetValue();
};

__interface IUnaryOperator : IOperator
{
	void eval(IOperand op);
};

__interface IBinaryOperator : IOperator
{
	void eval(IOperand op1, IOperand op2);
};