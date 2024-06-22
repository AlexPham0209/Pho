#include <iostream>
#include "Expression.h"

class Environment {
	private:
		std::map<std::string, TYPE> global;

	public:
		void createVariable(std::string name, TYPE value);
		void setVariable(std::string name, TYPE value);
		TYPE getVariable(std::string name);
};