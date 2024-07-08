#include "Expression.h"
#include "Lexer.h"
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
		Expression* call();
		Expression* finishCall(Expression* name);
		Expression* primary();
	
		Expression* declaration();
		Expression* ifStatement();
		Expression* whileLoop();
		Expression* forLoop();
		Expression* variableDeclaration();
		Expression* assignment();
		std::string identifier();
		
		TokenType consume(TokenType token, std::string message);
		bool check(TokenType token);

	public:
		Parser(std::vector<Token> tokens);
		std::vector<Expression*> createAST();
		
};