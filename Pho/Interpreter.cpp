#include "Interpreter.h"

Interpreter::Interpreter() : environment(new Environment()) {}

TYPE Interpreter::visitBinary(Binary* e) {
	TYPE leftVal = e->left->parse(this);
	TYPE rightVal = e->right->parse(this);
	TokenType sign = e->op;
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

	case Divisible:
		res = (double)((int)get<double>(leftVal) % (int)get<double>(rightVal));
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
	environment->createVariable(e->name, value);
	return TYPE();
}

TYPE Interpreter::visitVariable(Variable* e) {
	TYPE value = environment->getVariable(e->name);
	return value;
}

TYPE Interpreter::visitVariableAssign(VariableAssign* e) {
	TYPE value = e->value->parse(this);
	environment->setVariable(e->name, value);
	return TYPE();
}

TYPE Interpreter::visitBlock(Block* e) {
	Environment* prev = this->environment;
	this->environment = new Environment(prev);
	
	for (Expression* statement : e->statements) 
		statement->parse(this);
	
	
	delete this->environment;
	this->environment = prev;
	return TYPE();
}

TYPE Interpreter::visitIfStatement(IfStatement* e) {
	TYPE condition = e->condition->parse(this);

	if (!holds_alternative<bool>(condition))
		throw std::invalid_argument("No boolean arguments"); 
	
	if (get<bool>(condition))
		e->ifBlock->parse(this);
	else if (e->elseBlock != nullptr)
		e->elseBlock->parse(this);
	
	return TYPE();
}

TYPE Interpreter::visitWhileLoop(WhileLoop* e) {
	TYPE condition = e->condition->parse(this);

	while (holds_alternative<bool>(condition) && get<bool>(condition)) {
		e->block->parse(this);
		condition = e->condition->parse(this);
	}

	return TYPE();
}

TYPE Interpreter::visitFunctionCall(FunctionCall* e) {
	std::vector<TYPE> arguments;

	for (Expression* arg : e->arguments)
		arguments.push_back(arg->parse(this));
	
	for (TYPE arg : arguments) {
		if (holds_alternative<double>(arg))
			std::cout << get<double>(arg) << std::endl;

		else if (holds_alternative<bool>(arg))
			std::cout << (get<bool>(arg) ? "True" : "False") << std::endl;

		else if (holds_alternative<std::string>(arg))
			std::cout << get<std::string>(arg) << std::endl;
	}
	return TYPE();
}
