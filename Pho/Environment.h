#include <iostream>
#include "Expression.h"

class Environment {
	private:
		std::map<std::string, Expression*> global;

	public:
		void createVariable(std::string name, Expression* value);
		Expression* getVariable(std::string name);
};