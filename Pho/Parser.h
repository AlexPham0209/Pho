#include "Expression.h"
#include "Tokenizer.h"
#include "Error.h"

class Parser {
	private:
		std::vector<Token> tokens;
		int start;

		Expression* statement();
		Expression* blocking();
		Expression* equality();
		Expression* comparison();
		Expression* term();
		Expression* factor();
		Expression* unary();
		Expression* primary();
		void syntaxError(int line, std::string message);
		Expression* declaration();
		Expression* ifStatement();
		Expression* variableDeclaration();
		Expression* assignment();
		std::string identifier();
	
	public:
		Parser(std::vector<Token> tokens);
		std::vector<Expression*> createAST();
		
};