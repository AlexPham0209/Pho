#include "Environment.h"

void Environment::createVariable(std::string name, Expression* value) {
	global[name] = value;
}

Expression* Environment::getVariable(std::string name) {
	return global[name];
}
