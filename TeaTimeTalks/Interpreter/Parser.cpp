#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include "Value.h"
#include "Statement.h"
#include <list>

class Value;

void ReadFileContentsIntoString(std::ifstream& inStream, std::string& fileContents)
{
	std::stringstream strStream;
	strStream << inStream.rdbuf();
	fileContents = strStream.str();
}

void EatWhiteSpace(std::string& str, size_t& pos)
{
	while(pos<str.length() && isspace(str[pos]))
		++pos;
}

std::string PeekCharacter(std::string& str, size_t& pos)
{
	EatWhiteSpace(str, pos);
	return std::string(1, str[pos]);
}

std::string GetNextToken(std::string& str, size_t& pos)
{
	EatWhiteSpace(str, pos);
	if (pos >= str.length())
		return "";
	std::string ret;
	ret.push_back(str[pos]);
	++pos;
	if (isalnum(str[pos-1]))
		while (pos<str.length() && isalnum(str[pos]))
		{
			ret.push_back(str[pos]);
			++pos;
		}
	return ret;
}

std::string PeekToken(std::string& str, size_t& pos)
{
	size_t myPos = pos;
	return GetNextToken(str, myPos);
}

void Expect(std::string& str, size_t& pos, const std::string& val)
{
	std::string token = GetNextToken(str, pos);
	assert (token == val && "Syntax error");
	if (token != val)
	{
		std::cout << "Syntax error\n";
		exit(1);
	}
}

Constant* CreateConstant(std::string& val)
{
	int constVal = atoi(val.c_str());
	return new Constant(constVal);
}

Variable *CreateVariable(std::string& name)
{
	return new Variable(name);
}

bool IsOperator(std::string& op)
{
	return (op == "+") || (op == ">");
}

Value* CreateBinaryExpression(std::string& op, Value *lhs, Value *rhs)
{
	if (op == "+")
		return new AddOperator(lhs, rhs);
	else if (op == ">")
		return new GreaterThanOperator(lhs, rhs);
	else
		assert(0 && "Syntax error");
	return NULL;
}

Value *ParseExpression(std::string& str, size_t& pos);
Statement *ParseStatement(std::string& str, size_t& pos);

Value *ParseValue(std::string& str, size_t& pos)
{
	std::string token = GetNextToken(str, pos);
	if (isdigit(token[0]))
		return CreateConstant(token);
	if (token == "(")
	{
		Value *ret = ParseExpression(str, pos);
		Expect(str, pos, ")");
		return ret;
	}
	if (token == "IF")
	{
		Value *cond = ParseExpression(str, pos);
		Value *ifExp = ParseExpression(str, pos);
		Expect(str, pos, "ELSE");
		Value *elseExp = ParseExpression(str, pos);
		return new IfThenElse(cond, ifExp, elseExp);
	}
	if (isalpha(token[0]))
		return CreateVariable(token);

	assert(false && "Syntax error");
	return NULL;
}

// exp -> const | (exp) | exp op exp | IF exp exp ELSE exp
Value *ParseExpression(std::string& str, size_t& pos)
{
	Value *lhs = NULL, *rhs = NULL;
	lhs = ParseValue(str, pos);
	std::string s = PeekCharacter(str, pos);
	if (!IsOperator(s))
		return lhs;
	std::string op = GetNextToken(str, pos);
	rhs = ParseValue(str, pos);
	return CreateBinaryExpression(op, lhs, rhs);
}

ForLoop* ParseForLoop(std::string& str, size_t& pos)
{
	Expect(str, pos, "FOR");
	std::string loopVarName = GetNextToken(str, pos);
	Variable *loopVar = CreateVariable(loopVarName);
	Expect(str, pos, "=");
	Value* start = ParseExpression(str, pos);
	Expect(str, pos, ":");
	Value* step = ParseExpression(str, pos);
	Expect(str, pos, ":");
	Value* stop = ParseExpression(str, pos);

	ForLoop *forLoop = new ForLoop(loopVar, start, stop, step);

	std::string nextToken = PeekToken(str, pos);
	while (nextToken != "ENDFOR")
	{
		Statement *stm = ParseStatement(str, pos);
		forLoop->AddStatement(stm);
		nextToken = PeekToken(str, pos);
	}

	Expect(str, pos, "ENDFOR");
	return forLoop;
}

Statement *ParseStatement(std::string& str, size_t& pos)
{
	std::string token = PeekToken(str, pos);
	if (token == "FOR")
	{
		return ParseForLoop(str, pos);
	}
	else
	{
		Value *lhs = ParseExpression(str, pos);
		Expect(str, pos, "=");
		Value *rhs = ParseExpression(str, pos);
		Expect(str, pos, ";");
		return new AssignmentStatement(lhs, rhs);
	}
	return NULL;
}

std::list<Statement*> ParseFile(const std::string& fileName)
{
	std::ifstream inStream(fileName.c_str());
	assert (!inStream.bad());
	std::string fileContents;
	ReadFileContentsIntoString(inStream, fileContents);
	size_t pos = 0;
	//EatWhiteSpace(fileContents, pos);
	//std::string token = GetNextToken(fileContents, pos);
	//while (token != "")
	//{
	//	std::cout << token << std::endl;
	//	token = GetNextToken(fileContents, pos);
	//}
	std::list<Statement*> statementList;
	while (pos < fileContents.length())
	{
		Statement *stm = ParseStatement(fileContents, pos);
		statementList.push_back(stm);
		EatWhiteSpace(fileContents, pos);
	}
	return statementList;
}