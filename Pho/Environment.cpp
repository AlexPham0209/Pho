#include "Environment.h"

Environment::Environment() : prev(nullptr) {}

Environment::Environment(Environment* prev) : prev(prev) {}

void Environment::createVariable(std::string name, TYPE value) {
	if (!values.count(name))
		values[name] = value;
	
	else if (prev != nullptr) 
		prev->createVariable(name, value);

	else
		throw std::invalid_argument("bruh");
}

void Environment::setVariable(std::string name, TYPE value) {
	if (values.count(name)) {
		values[name] = value;
		return;
	}

	if (prev != nullptr) {
		prev->setVariable(name, value);
		return;
	}
	
	throw std::invalid_argument("No variable");
}

TYPE Environment::getVariable(std::string name) {
	if (values.count(name)) 
		return values[name];

	if (prev != nullptr) 
		return prev->getVariable(name);

	throw std::invalid_argument("No variable");
}

void Environment::createFunction(std::string name, FunctionDeclaration* function) {
	if (!functions.count(name) && function != nullptr)
		functions[name] = function;
}

FunctionDeclaration* Environment::getFunction(std::string name) {
	if (functions.count(name))
		return functions[name];

	if (prev != nullptr)
		return prev->getFunction(name);

	throw std::invalid_argument("No function");
}


