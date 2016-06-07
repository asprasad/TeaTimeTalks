#include "stdafx.h"
#include <iostream>
#include <cassert>
#include <algorithm>
#include "Value.h"
#include "Statement.h"

void AssignmentStatement::Execute()
{
	Variable *var = dynamic_cast<Variable*>(m_lhs);
	assert (var && "LHS must be a variable");
	int rhsValue = m_rhs->Evaluate();
	var->assignValue(rhsValue);
}

void AssignmentStatement::Print()
{
	m_lhs->Print();
	std::cout << " = ";
	m_rhs->Print();
	std::cout << ";\n";
}

void ForLoop::Execute()
{
	int start = m_start->Evaluate();
	int stop = m_stop->Evaluate();
	int step = m_step->Evaluate();
	for (int i=start ; i<=stop ; i+=step)
	{
		Variable* loopVar = dynamic_cast<Variable*>(m_loopVar);
		assert (loopVar && "Loop must have a variable");
		loopVar->assignValue(i);
		std::for_each(m_stmList.begin(), m_stmList.end(), [](Statement *stm) { stm->Execute(); });
	}
}

void ForLoop::Print()
{
	std::cout << "FOR ";
	m_loopVar->Print();
	std::cout << " = ";
	m_start->Print();
	std::cout << " : ";
	m_step->Print();
	std::cout << " : ";
	m_stop->Print();
	std::cout << "\n";

	std::for_each(m_stmList.begin(), m_stmList.end(), [](Statement *stm) { stm->Print(); });

	std::cout << "END FOR\n";
}