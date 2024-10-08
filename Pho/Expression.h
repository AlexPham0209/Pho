#pragma once
#include <variant>
#include <map>
#include <any>
#include <sstream>
#include <format>
#include "Lexer.h"
#define TYPE std::variant<double, bool, std::string>

enum DataType {
	DOUBLE,
	STRING,
	BOOL,
	NONE,
};

class Visitor;
class Expression {
	public:
		Expression() {}
		~Expression() {}
		virtual TYPE parse(Visitor* v) = 0;
};

class Print : public Expression {
	public:
		Expression* expression;

		Print(Expression* expression);
		~Print() {
			delete expression;
		} 

		TYPE parse(Visitor* v);
};

class Binary : public Expression {
	public:
		Expression* left;
		Expression* right;
		TokenType op;

		Binary(Expression* left, Expression* right, TokenType op);
		~Binary() {
			delete left;
			delete right;
		}

		TYPE parse(Visitor* v);
};

class Unary : public Expression {
	public:
		Expression* expression;
		TokenType op;

		Unary(Expression* expression, TokenType op);
		~Unary() {
			delete expression;
		}

		TYPE parse(Visitor* v);
};

class Literal : public Expression {
	public:
		TYPE val;
		DataType dataType;
	
		Literal(TYPE val);
		Literal(double val);
		Literal(bool val);
		Literal(std::string val);
	
		TYPE parse(Visitor* v) override;
};

class Grouping : public Expression {
	public:
		Expression* expression;

		Grouping(Expression* expression);
		TYPE parse(Visitor* v) override;
};

class VariableDeclaration : public Expression {
	public:
		std::string name;
		Expression* value;

		VariableDeclaration(std::string name, Expression* value);
		~VariableDeclaration() {
			delete value;
		}

		TYPE parse(Visitor* v) override;
};

class Variable : public Expression {
	public:
		std::string name;

		Variable(std::string name);
		TYPE parse(Visitor* v) override;
};


class VariableAssign : public Expression {
public:
	std::string name;
	Expression* value;

	VariableAssign(std::string name, Expression* value);
	TYPE parse(Visitor* v) override;
};

class Block : public Expression {
	public:
		std::vector<Expression*> statements;

		Block(std::vector<Expression*> statements);
		TYPE parse(Visitor* v) override;
};

class IfStatement : public Expression {
	public:
		Expression* condition;
		Expression* ifBlock;
		Expression* elseBlock;

		IfStatement(Expression* condition, Expression* ifBlock, Expression* elseBlock);
		~IfStatement() {
			delete condition;
			delete ifBlock;
			delete elseBlock;
		}

		TYPE parse(Visitor* v) override;
};

class WhileLoop : public Expression {
	public:
		Expression* condition;
		Expression* block;

		WhileLoop(Expression* condition, Expression* block);
		TYPE parse(Visitor* v) override;
};

class FunctionDeclaration : public Expression {
	public:
		Token name;
		std::vector<Expression*> arguments;
		Block* body;

		FunctionDeclaration(Token name, std::vector<Expression*> arguments, Block* body);
		TYPE parse(Visitor* v) override;
};

class FunctionCall : public Expression {
	public:
		Expression* name;
		TokenType paren;
		std::vector<Expression*> arguments;


		FunctionCall(Expression* name, TokenType paren, std::vector<Expression*> arguments);
		TYPE parse(Visitor* v) override;
};

class ReturnStatement : public Expression {
	public:
		Expression* value;

		ReturnStatement(Expression* value);
		TYPE parse(Visitor* v) override;

};