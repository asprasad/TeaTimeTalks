#pragma once

#include <list>

class Value;
class Variable;

class Statement
{
public:
	virtual void Execute() = 0;
	virtual void Print() = 0;
};

class AssignmentStatement : public Statement
{
	Value *m_lhs;
	Value *m_rhs;
public:
	AssignmentStatement(Value *lhs, Value *rhs)
		: m_lhs(lhs), m_rhs(rhs)
	{ }
	virtual void Execute();
	virtual void Print();
};

class ForLoop : public Statement
{
	std::list<Statement*> m_stmList;
	Value *m_start;
	Value *m_stop;
	Value *m_step;
	Value *m_loopVar;
public:
	ForLoop(Value *loopVar, Value *start, Value *stop, Value *step)
		:m_start(start), m_stop(stop), m_step(step), m_loopVar(loopVar)
	{ }
	virtual void Execute();
	virtual void Print();
	void AddStatement(Statement *stm) { m_stmList.push_back(stm); }
};