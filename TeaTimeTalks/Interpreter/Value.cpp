#include "stdafx.h"
#include "Value.h"
#include <iostream>
#include <map>
#include <cassert>

static std::map<std::string, int> sVariableValues;

int Constant::Evaluate() 
{ 
	return m_val;
}

void Constant::Print()
{ 
	std::cout << m_val;
}

int Variable::Evaluate() 
{ 
	std::map<std::string, int>::iterator iter = sVariableValues.find(m_name);
	assert (iter != sVariableValues.end() && "Using an uninitialized value");
	return iter->second;
}

void PrintVariableValues()
{
	for (std::map<std::string, int>::iterator iter = sVariableValues.begin() ; iter!=sVariableValues.end() ; ++iter)
		std::cout << iter->first << " : " << iter->second << std::endl;
}

void Variable::assignValue(int val)
{
	sVariableValues[m_name] = val;
}

void Variable::Print()
{ 
	std::cout << m_name;
}

int AddOperator::Evaluate()
{
	int rhs = m_rhs->Evaluate();
	int lhs = m_lhs->Evaluate();
	return rhs + lhs;
}

void AddOperator::Print()
{
	std::cout << "(";
	m_lhs->Print();
	std::cout << " + ";
	m_rhs->Print();
	std::cout << ")";
}

int GreaterThanOperator::Evaluate()
{
	int rhs = m_rhs->Evaluate();
	int lhs = m_lhs->Evaluate();
	return lhs > rhs;
}
	
void GreaterThanOperator::Print()
{
	std::cout << "(";
	m_lhs->Print();
	std::cout << " > ";
	m_rhs->Print();
	std::cout << ")";
}

int IfThenElse::Evaluate()
{
	int condition = m_cond->Evaluate();
	int ifValue = m_ifValue->Evaluate();
	int elseValue = m_elseValue->Evaluate();
	return condition ? ifValue : elseValue;
}
	
void IfThenElse::Print()
{
	std::cout << "(";
	std::cout << "IF (";
	m_cond->Print();
	std::cout << ") ";
	m_ifValue->Print();
	std::cout << " ELSE ";
	m_elseValue->Print();
	std::cout << ")";
}

