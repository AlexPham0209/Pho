#pragma once
#include <variant>
#include <map>
#include <any>
#include <sstream>
#include <format>
#include "Tokenizer.h"
#define TYPE std::variant<double, bool, std::string>

enum DataType {
	FLOAT,
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
		Literal(float val);
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


class VariableSet : public Expression {
public:
	std::string name;
	Expression* value;

	VariableSet(std::string name, Expression* value);
	TYPE parse(Visitor* v) override;
};