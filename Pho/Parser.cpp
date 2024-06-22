#include "Parser.h"

Parser::Parser(std::vector<Token> tokens) : tokens(tokens), start(0) {}

std::vector<Expression*> Parser::createAST() {
	std::vector<Expression*> statements;
	
	while (start < tokens.size() && tokens[start].type != EndOfFile) 
		statements.push_back(declaration());
	
	return statements;
}

Expression* Parser::declaration() {
	if (start >= tokens.size())
		return nullptr;

	if (tokens[start].type == Set) 
		return variableDeclaration();

	return statement();
}

Expression* Parser::variableDeclaration() {
	std::string name = identifier();
	start++;
	Expression* variable;

	if (start >= tokens.size())
		throw std::invalid_argument("end");

	//Checking if there is a set function
	if (tokens[start].type != Equal) {
		Expression* value = (Expression*)(new Literal((double)NULL));
		return new VariableDeclaration(name, value);
	}

	start++;
	return new VariableDeclaration(name, equality());
}

Expression* Parser::assignment() {
	Expression* e = equality();
	
	if (start <= tokens.size() && tokens[start].type == Equal) {
		start++;
		Expression* value = assignment();

		if (Variable* v = dynamic_cast<Variable*>(e)) {
			Expression* assign = new VariableAssign(v->name, value);
			return assign;
		}

		throw std::invalid_argument("Not a variable");
	}

	return e;
}

std::string Parser::identifier() {
	start++;
	if (start < tokens.size() && tokens[start].type != Identifier)
		throw std::invalid_argument("Expect variable name");

	return tokens[start].value;
}

Expression* Parser::statement() {
	if (start >= tokens.size())
		return nullptr;

	if (tokens[start].type == PrintStatement) {
		start++;
		Expression* expression = assignment();
		Print* print = new Print(expression);
		return print;
	}

	if (tokens[start].type == OpenCurly)
		return blocking();

	return assignment();
}

Expression* Parser::blocking() {
	start++;
	std::vector<Expression*> statements;
	while (start < tokens.size() && (tokens[start].type != ClosedCurly && tokens[start].type != EndOfFile)) {
		std::cout << tokens[start].value << std::endl;
		statements.push_back(declaration());
	}
	
	start++;
	Block* blocking = new Block(statements);
	return blocking;
}

Expression* Parser::equality() {
	Expression* res = comparison();

	while (start < tokens.size() && (tokens[start].type == NotEqual || tokens[start].type == EqualCompare)) {
		TokenType op = tokens[start].type;
		start++;
		Expression* other = comparison();
		res = new Binary(res, other, op);
	}

	return res;
}

Expression* Parser::comparison() {
	Expression* res = term();

	while (start < tokens.size() && (tokens[start].type == Greater || tokens[start].type == GreaterEqual || tokens[start].type == Less || tokens[start].type == LessEqual || tokens[start].type == And || tokens[start].type == Or)) {
		TokenType op = tokens[start].type;
		start++;
		Expression* other = term();
		res = new Binary(res, other, op);
	}

	return res;
}

Expression* Parser::term() {
	Expression* res = factor();

	while (start < tokens.size() && (tokens[start].type == Plus || tokens[start].type == Minus)) {
		TokenType op = tokens[start].type;
		start++;

		Expression* other = factor();
		res = new Binary(res, other, op);
	}

	return res;
}

Expression* Parser::factor() {
	Expression* res = unary();

	while (start < tokens.size() && (tokens[start].type == Divide || tokens[start].type == Multiply)) {
		TokenType op = tokens[start].type;
		start++;

		Expression* other = unary();
		res = new Binary(res, other, op);
	}

	return res;
}

Expression* Parser::unary() {
	//If the next token is another unary operator
	if (start < tokens.size() && (tokens[start].type == Minus || tokens[start].type == Not)) {
		TokenType op = tokens[start].type;
		start++;

		Expression* expression = unary();
		Expression* res = new Unary(expression, op);
		return res;
	}

	return primary();
}

Expression* Parser::primary() {
	Token token = tokens[start];
	start++;

	switch (token.type) {
		case Number:
			return new Literal(std::stof(token.value));
		
		case String:
			return new Literal(token.value);
		
		case True:
			return new Literal(true);

		case False:
			return new Literal(false);

		case Identifier:
			return new Variable(token.value);

		case OpenParenthesis:
			Expression* exp = equality();

			//Consume closed parenthesis
			if (tokens[start].type == ClosedParenthesis)
				start++;

			Expression* res = new Grouping(exp);
			return res;
	}

	throw std::invalid_argument("Invalid literal");
}
