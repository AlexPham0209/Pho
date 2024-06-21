#include "Visitor.h"

std::map<TokenType, std::string> operationMap = {
	{Plus, "+"},
	{Minus, "-"},
	{Multiply, "*"},
	{Divide, "/"},
	{Not, "!"},
	{EqualCompare, "=="},
	{NotEqual, "!="},
	{Less, "<"},
	{LessEqual, "<="},
	{Greater, ">"},
	{GreaterEqual, ">="},
	{And, "&&"},
	{Or, "||"},
};

TYPE PrintVisitor::visitBinary(Binary* e) {
	std::stringstream stream;
	TYPE leftVal = e->left->parse(this);
	TYPE rightVal = e->right->parse(this);
	std::string sign = operationMap[e->op];
	
	if (!holds_alternative<std::string>(leftVal) || !holds_alternative<std::string>(rightVal))
		return TYPE();

	stream << get<std::string>(leftVal) << " " << get<std::string>(rightVal) << " " << sign;

	return TYPE(stream.str());
}

TYPE PrintVisitor::visitUnary(Unary* e) {
	std::stringstream stream;
	TYPE val = e->expression->parse(this);

	if (!holds_alternative<std::string>(val))
		return TYPE();

	stream << operationMap[e->op] << get<std::string>(val) << " ";

	return TYPE(stream.str());
}

TYPE PrintVisitor::visitLiteral(Literal* e) {
	std::string val;

	if (holds_alternative<double>(e->val))
		val = std::format("{:.2f}", get<double>(e->val));

	else if (holds_alternative<bool>(e->val))
		val = std::to_string(get<bool>(e->val));

	else if (holds_alternative<std::string>(e->val))
		val = get<std::string>(e->val);

	return TYPE(val);
}
