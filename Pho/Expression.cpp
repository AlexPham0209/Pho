#include "Expression.h"
#include "Visitor.h"

Print::Print(Expression* expression) : expression(expression) {}

Binary::Binary(Expression* left, Expression* right, TokenType op) : left(left), right(right), op(op) {}
Unary::Unary(Expression* expression, TokenType op) : expression(expression), op(op) {}

Literal::Literal(TYPE val) : val(val), dataType(NONE) {}
Literal::Literal(double val) : val(val), dataType(DOUBLE) {}
Literal::Literal(bool val) : val(val), dataType(BOOL) {}
Literal::Literal(std::string val) : val(val), dataType(STRING) {}

Grouping::Grouping(Expression* expression) : expression(expression) {}

Variable::Variable(std::string name) : name(name) {}
VariableDeclaration::VariableDeclaration(std::string name, Expression* value) : name(name), value(value) {}
VariableAssign::VariableAssign(std::string name, Expression* value) : name(name), value(value) {}

Block::Block(std::vector<Expression*> statements) : statements(statements) {}
IfStatement::IfStatement(Expression* condition, Expression* ifBlock, Expression* elseBlock) : condition(condition), ifBlock(ifBlock), elseBlock(elseBlock) {}

WhileLoop::WhileLoop(Expression* condition, Expression* block) : condition(condition), block(block) {}

FunctionCall::FunctionCall(Expression* name, TokenType paren, std::vector<Expression*> arguments) : name(name), paren(paren), arguments(arguments) {}
FunctionDeclaration::FunctionDeclaration(Token name, std::vector<Expression*> arguments, Block* body) : name(name), arguments(arguments), body(body) {}
ReturnStatement::ReturnStatement(Expression* value) : value(value) {}

TYPE Print::parse(Visitor* v) {
	return v->visitPrint(this);
}
TYPE Binary::parse(Visitor* v) {
	return v->visitBinary(this);
}

TYPE Unary::parse(Visitor* v) {
	return v->visitUnary(this);
}

TYPE Literal::parse(Visitor* v) {
	return v->visitLiteral(this);
}

TYPE Grouping::parse(Visitor* v) {
	return v->visitGrouping(this);
}

TYPE VariableDeclaration::parse(Visitor* v) {
	return v->visitVariableDeclaration(this);
}

TYPE Variable::parse(Visitor* v) {
	return v->visitVariable(this);
}
TYPE VariableAssign::parse(Visitor* v) {
	return v->visitVariableAssign(this);
}

TYPE Block::parse(Visitor* v) {
	return v->visitBlock(this);
}

TYPE IfStatement::parse(Visitor* v) {
	return v->visitIfStatement(this);
}

TYPE WhileLoop::parse(Visitor* v) {
	return v->visitWhileLoop(this);
}

TYPE FunctionCall::parse(Visitor* v) {
	return v->visitFunctionCall(this);
}

TYPE FunctionDeclaration::parse(Visitor* v) {
	return v->visitFunctionDeclaration(this);
}

TYPE ReturnStatement::parse(Visitor* v) {
	return v->visitReturnStatement(this);
}
