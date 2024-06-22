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
