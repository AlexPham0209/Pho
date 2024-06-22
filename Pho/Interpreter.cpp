#include "Interpreter.h"

TYPE Interpreter::visitBinary(Binary* e) {
	TYPE leftVal = e->left->parse(this);
	TYPE rightVal = e->right->parse(this);
	TokenType sign = e->op;

	if (!holds_alternative<double>(leftVal) || !holds_alternative<double>(rightVal))
		return TYPE();
	TYPE res;

	switch (sign) {
	case Plus:
		res = get<double>(leftVal) + get<double>(rightVal);
		break;

	case Minus:
		res = get<double>(leftVal) - get<double>(rightVal);
		break;

	case Multiply:
		res = get<double>(leftVal) * get<double>(rightVal);
		break;

	case Divide:
		res = get<double>(leftVal) / get<double>(rightVal);
		break;

	case GreaterEqual:
		res = (get<double>(leftVal) >= get<double>(rightVal));
		break;

	case Greater:
		res = (get<double>(leftVal) > get<double>(rightVal));
		break;

	case LessEqual:
		res = (get<double>(leftVal) <= get<double>(rightVal));
		break;

	case Less:
		res = (get<double>(leftVal) < get<double>(rightVal));
		break;

	case EqualCompare:
		res = (get<double>(leftVal) == get<double>(rightVal));
		break;

	case NotEqual:
		res = (get<double>(leftVal) != get<double>(rightVal));
		break;

	case And:
		res = (bool)(get<bool>(leftVal) && get<bool>(rightVal));
		break;

	case Or:
		res = (bool)(get<bool>(leftVal) || get<bool>(rightVal));
		break;
	}

	return res;
}

TYPE Interpreter::visitUnary(Unary* e) {
	TYPE exp = e->expression->parse(this);

	if (e->op == Not && holds_alternative<bool>(exp))
		return TYPE((bool)(!get<bool>(exp)));

	else if (e->op == Minus && holds_alternative<double>(exp))
		return TYPE(-get<double>(exp));

	return TYPE();
}

TYPE Interpreter::visitLiteral(Literal* e) {
	return e->val;
}

TYPE Interpreter::visitGrouping(Grouping* e) {
	TYPE res = e->expression->parse(this);
	return res;
}

TYPE Interpreter::visitPrint(Print* e) {
	TYPE expression = e->expression->parse(this);
	if (holds_alternative<double>(expression))
		std::cout << get<double>(expression) << std::endl;

	else if (holds_alternative<bool>(expression))
		std::cout << (get<bool>(expression) ? "True" : "False") << std::endl;

	else if (holds_alternative<std::string>(expression))
		std::cout << get<std::string>(expression) << std::endl;

	return TYPE();
}

TYPE Interpreter::visitVariableDeclaration(VariableDeclaration* e) {
	TYPE value = e->value->parse(this);
	environment.createVariable(e->name, value);
	return TYPE();
}

TYPE Interpreter::visitVariable(Variable* e) {
	TYPE value = environment.getVariable(e->name);
	return value;
}

TYPE Interpreter::visitVariableSet(VariableSet* e) {
	TYPE value = e->value->parse(this);
	environment.setVariable(e->name, value);
	return TYPE();
}
