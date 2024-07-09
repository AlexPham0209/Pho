#include <iostream>
#include "Expression.h"

class Environment {
	private:
		std::map<std::string, TYPE> values;
		std::map<std::string, FunctionDeclaration*> functions;

	public:
		Environment* prev;
		Environment();
		Environment(Environment* prev);
		void createVariable(std::string name, TYPE value);
		void setVariable(std::string name, TYPE value);
		TYPE getVariable(std::string name);

		void createFunction(std::string name, FunctionDeclaration* function);
		FunctionDeclaration* getFunction(std::string name);
};