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
	try {
		environment->createVariable(e->name, value);
	}
	catch (RuntimeError& e) {
		std::cout << e.what() << std::endl;
	}
	return TYPE();
}

TYPE Interpreter::visitVariable(Variable* e) {
	TYPE value;
	try {
		value = environment->getVariable(e->name);
	}
	catch (RuntimeError& e) {
		std::cout << e.what() << std::endl;
	}
	return value;
}

TYPE Interpreter::visitVariableAssign(VariableAssign* e) {
	TYPE value = e->value->parse(this);
	try {
		environment->setVariable(e->name, value);
	}
	catch (RuntimeError& e) {
		std::cout << e.what() << std::endl;
	}

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
		throw new RuntimeError("No boolean arguments"); 
	
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
	std::map<std::string, TYPE> arguments;
	
	std::string name = dynamic_cast<Variable*>(e->name)->name;
	FunctionDeclaration* fun = environment->getFunction(name);

	for (int i = 0; i < e->arguments.size(); ++i)
		arguments[dynamic_cast<Variable*>(fun->arguments[i])->name] = e->arguments[i]->parse(this);

	Environment* prev = this->environment;
	this->environment = new Environment(prev);

	for (auto i = arguments.begin(); i != arguments.end(); ++i) 
		this->environment->createVariable(i->first, i->second);
	
	Block* body = fun->body;

	TYPE ret = TYPE();

	for (Expression* statement : body->statements) {
		if (typeid(*statement) != typeid(ReturnStatement))
			statement->parse(this);
		else {
			ret = statement->parse(this);
			break;
		}
	}

	delete this->environment;
	this->environment = prev;
	return ret;
}

TYPE Interpreter::visitFunctionDeclaration(FunctionDeclaration* e) {
	environment->createFunction(e->name.value, e);
	return TYPE();
}

TYPE Interpreter::visitReturnStatement(ReturnStatement* e) {
	return e->value->parse(this);
}
