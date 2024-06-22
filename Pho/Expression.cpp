#include "Expression.h"
#include "Visitor.h"

Print::Print(Expression* expression) : expression(expression) {}

Binary::Binary(Expression* left, Expression* right, TokenType op) : left(left), right(right), op(op) {}
Unary::Unary(Expression* expression, TokenType op) : expression(expression), op(op) {}

Literal::Literal(TYPE val) : val(val), dataType(NONE) {}
Literal::Literal(float val) : val(val), dataType(FLOAT) {}
Literal::Literal(bool val) : val(val), dataType(BOOL) {}
Literal::Literal(std::string val) : val(val), dataType(STRING) {}

Grouping::Grouping(Expression* expression) : expression(expression) {}

Variable::Variable(std::string name) : name(name) {}
VariableDeclaration::VariableDeclaration(std::string name, Expression* value) : name(name), value(value) {}
VariableSet::VariableSet(std::string name, Expression* value) : name(name), value(value) {}


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
TYPE VariableSet::parse(Visitor* v) {
	return v->visitVariableSet(this);
}
