#include <iostream>
#include "Expression.h"

class Environment {
	private:
		std::map<std::string, TYPE> values;
		Environment* prev;

	public:
		Environment();
		Environment(Environment* prev);
		void createVariable(std::string name, TYPE value);
		void setVariable(std::string name, TYPE value);
		TYPE getVariable(std::string name);
};