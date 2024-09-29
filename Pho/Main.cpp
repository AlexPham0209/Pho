// Pho.cpp : Defines the entry point for the application.
//

#include "Main.h"
#include "Expression.h"
#include "Visitor.h"
#include "Parser.h"
#include "Interpreter.h"

std::string readFile(std::string path) {
	std::string text;
	std::string line;
	std::ifstream file(path);

	if (file.is_open()) {
		while (file.good()) {
			getline(file, line);
			text += line + "\n";
		}
	}

	file.close();
	return text;
}

//Very basic interpreter
int main(int argc, const char** argv) {
	if (argc <= 1) {
		std::cout << "No file" << std::endl;
		return -1;
	}
	//User input
	std::string file(argv[1]);

	//Converts the inputted string into a series of tokens
	Lexer lexer(readFile(file));
	std::vector<Token> tokens = lexer.tokenize();

	//Parses the resulting tokens and creates an Abstract Syntax Tree 
	Parser parser(tokens);
	std::vector<Expression*> statements;

	try {
		statements = parser.createAST();
	}
	catch (SyntaxError& e) {
		std::cout << e.what() << std::endl;
	}

	//By using the visitor pattern, we are able to create derived vistor objects that can explore the resulting tree and print out result
	Interpreter interpreter;

	for (Expression* statement : statements) {
		try {
			statement->parse((Visitor*)&interpreter);
		}
		catch (RuntimeError& e) {
			std::cout << e.what() << std::endl;
		}
	}

	for (Expression* statement : statements)
		delete statement;

	return 0;
}

