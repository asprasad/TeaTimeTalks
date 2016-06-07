#pragma once

#include <string>

class Value
{
public:
	virtual int Evaluate() = 0;
	virtual void Print() = 0;
};

class Constant : public Value
{
	int m_val;
public:
	Constant(int val) : m_val(val) { }
	virtual int Evaluate();
	virtual void Print();
};

class Variable : public Value
{
	std::string m_name;
public:
	Variable(const std::string& name)
		:m_name(name)
	{ }
	virtual int Evaluate();
	virtual void Print();
	void assignValue(int val);
};

class AddOperator : public Value
{
	Value *m_rhs;
	Value *m_lhs;
public:
	AddOperator(Value *lhs, Value* rhs)
		: m_rhs(rhs), m_lhs(lhs)
	{ }
	virtual int Evaluate();
	virtual void Print();
};

class GreaterThanOperator : public Value
{
	Value *m_rhs;
	Value *m_lhs;
public:
	GreaterThanOperator(Value *lhs, Value* rhs)
		: m_rhs(rhs), m_lhs(lhs)
	{ }
	virtual int Evaluate();
	virtual void Print();
};

class IfThenElse : public Value
{
	Value *m_cond;
	Value *m_ifValue;
	Value *m_elseValue;
public:
	IfThenElse(Value *cond, Value* ifValue, Value *elseValue) :m_cond(cond), m_ifValue(ifValue), m_elseValue(elseValue) { }
	virtual int Evaluate();
	virtual void Print();
};
