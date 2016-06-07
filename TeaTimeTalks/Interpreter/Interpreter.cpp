// Interpreter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <list>
#include <algorithm>
#include "Value.h"
#include "Statement.h"

std::list<Statement*> ParseFile(const std::string& fileName);
void PrintVariableValues();

int _tmain(int argc, _TCHAR* argv[])
{
	//Constant one(1);
	//Constant two(2);
	//Constant fortyTwo(42);
	//Constant fortyThree(43);
	//GreaterThanOperator gtOperator(&fortyTwo, &fortyThree);

	//IfThenElse ifVal(&gtOperator, &two, &one);
	//AddOperator add(&ifVal, &two);

	//add.Print();
	//std::cout << std::endl;

	//std::cout << add.Evaluate() << std::endl;
	std::list<Statement*> stmList = ParseFile("c:\\testFile.txt");
	std::for_each(stmList.begin(), stmList.end(), [](Statement* stm) { stm->Print(); });
	std::for_each(stmList.begin(), stmList.end(), [](Statement* stm) { stm->Execute(); });
	PrintVariableValues();
	// exp->Print();
	// std::cout << std::endl << exp->Evaluate() << std::endl;
	return 0;
}

