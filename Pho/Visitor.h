#pragma once
#include <string>
#include <variant>
#include <iostream>
#include "Expression.h"
#include "Lexer.h"

class Visitor {
	public:
		Visitor() {}
		~Visitor() {}
		virtual TYPE visitBinary(Binary* e) = 0;
		virtual TYPE visitUnary(Unary* e) = 0;
		virtual TYPE visitLiteral(Literal* e) = 0;
		virtual TYPE visitGrouping(Grouping* e) = 0;
		virtual TYPE visitPrint(Print* e) = 0;
		virtual TYPE visitVariableDeclaration(VariableDeclaration* e) = 0;
		virtual TYPE visitVariable(Variable* e) = 0;
		virtual TYPE visitVariableAssign(VariableAssign* e) = 0;
		virtual TYPE visitBlock(Block* e) = 0;
		virtual TYPE visitIfStatement(IfStatement* e) = 0;
		virtual TYPE visitWhileLoop(WhileLoop* e) = 0;
		virtual TYPE visitFunctionCall(FunctionCall* e) = 0;
		virtual TYPE visitFunctionDeclaration(FunctionDeclaration* e) = 0;
		virtual TYPE visitReturnStatement(ReturnStatement* e) = 0;
};

class PrintVisitor : Visitor {
	public:
		TYPE visitBinary(Binary* e) override;
		TYPE visitUnary(Unary* e) override;
		TYPE visitLiteral(Literal* e) override;
		TYPE visitGrouping(Grouping* e) override;
		TYPE visitPrint(Print* e) override;
};
