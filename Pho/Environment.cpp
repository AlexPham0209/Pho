#include "Environment.h"

void Environment::createVariable(std::string name, TYPE value) {
	global[name] = value;
}

void Environment::setVariable(std::string name, TYPE value) {
	if (!global.count(name))
		throw std::invalid_argument("Variable not initialized");
	global[name] = value;
}

TYPE Environment::getVariable(std::string name) {
	return global[name];
}


