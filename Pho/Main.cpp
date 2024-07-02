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
int main() {
	//User input
	std::string problem;
	/*std::cout << "Enter problem: ";
	std::getline(std::cin, problem, '\n');
	std::cout << std::endl;*/

	//Converts the inputted string into a series of tokens
	Lexer lexer(readFile("C:/Users/RedAP/Desktop/Programming Projects/Pho/ConditionalTest.txt"));
	std::vector<Token> tokens = lexer.tokenize();

	//for (Token token : tokens)
	//	std::cout << token.value << ", " << token.type << std::endl;

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

	for (Expression* statement : statements)
		statement->parse((Visitor*)&interpreter);

	for (Expression* statement : statements)
		delete statement;

	return 0;
}

