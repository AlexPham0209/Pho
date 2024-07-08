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

Expression* Parser::statement() {
	if (start >= tokens.size())
		throw SyntaxError(0, "At end");
	
	switch (tokens[start].type) {
		case OpenCurly:
			return blocking();

		case If:
			return ifStatement();

		case While:
			return whileLoop();

		case For:
			return forLoop();

		case PrintStatement:
			start++;
			Expression* expression = assignment();
			Print* print = new Print(expression);
			return print;
	}

	return assignment();
}

Expression* Parser::ifStatement() {
	start++;

	//Getting conditions
	consume(OpenBracket, "No open bracket");
	Expression* condition = equality();
	consume(ClosedBracket, "No closed bracket");

	//Creating then block
	Block* block = (Block*)blocking();

	//Create else statement
	Block* elseBlock = nullptr;
	if (tokens[start].type == Else) {
		start++;
		elseBlock = (Block*)blocking();
	}

	//Instantiating if statement object that is allocated to the heap
	IfStatement* statement = new IfStatement(condition, block, elseBlock);
	return statement;
}

Expression* Parser::whileLoop() {
	start++;

	//Getting conditions
	consume(OpenBracket, "No open bracket");
	Expression* condition = equality();
	consume(ClosedBracket, "No closed bracket");

	Block* block = (Block*)blocking();

	WhileLoop* loop = new WhileLoop(condition, block);
	return loop;
}

Expression* Parser::forLoop() {
	start++;
	consume(OpenBracket, "No open bracket");
	Expression* initializer; 

	//Set initializer statement
	if (tokens[start].type == Colon)
		initializer = nullptr;
	else if (tokens[start].type == Set)
		initializer = variableDeclaration();
	else
		initializer = equality();
	
	//Set condition 
	consume(Colon, "Expect ':' after for clauses");
	Expression* condition = equality();

	//Set increment
	consume(Colon, "Expect ':' after for clauses");
	Expression* increment = nullptr;
	if (tokens[start].type != ClosedBracket) 
		increment = declaration();
	consume(ClosedBracket, "Expect ']' after increment");
	
	//Create block
	Block* block = (Block*)blocking();

	//Assembling while loop
	std::vector<Expression*> body = {block, increment};
	WhileLoop* forBody = new WhileLoop(condition, new Block(body));

	std::vector<Expression*> res = {initializer, forBody};
	return new Block(res);
}

Expression* Parser::blocking() {
	if (tokens[start].type != OpenCurly)
		throw SyntaxError(tokens[start].line, "No block");

	start++;
	std::vector<Expression*> statements;
	while (start < tokens.size() && (tokens[start].type != ClosedCurly && tokens[start].type != EndOfFile))
		statements.push_back(declaration());

	start++;
	Block* blocking = new Block(statements);
	return blocking;
}

Expression* Parser::variableDeclaration() {
	std::string name = identifier();
	start++;
	Expression* variable;

	if (start >= tokens.size())
		throw SyntaxError(tokens[start].line, "At end");

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
	
	if (check(Equal)) {
		start++;
		Expression* value = assignment();

		if (Variable* v = dynamic_cast<Variable*>(e)) {
			Expression* assign = new VariableAssign(v->name, value);
			return assign;
		}

		throw SyntaxError(tokens[start].line, "Not valid variable");
	}

	return e;
}

std::string Parser::identifier() {
	start++;
	if (start < tokens.size() && tokens[start].type != Identifier) 
		throw SyntaxError(tokens[start].line, "Not an identifier");
	
	return tokens[start].value;
}


Expression* Parser::equality() {
	Expression* res = comparison();

	while (check(NotEqual) || check(EqualCompare)) {
		TokenType op = tokens[start].type;
		start++;
		Expression* other = comparison();
		res = new Binary(res, other, op);
	}

	return res;
}

Expression* Parser::comparison() {
	Expression* res = term();

	while (check(Less) || check(LessEqual) || check(Greater) || check(GreaterEqual) || check(And) || check(Or) || check(Divisible)) {
		TokenType op = tokens[start].type;
		start++;
		Expression* other = term();
		res = new Binary(res, other, op);
	}

	return res;
}

Expression* Parser::term() {
	Expression* res = factor();

	while (check(Plus) || check(Minus)) {
		TokenType op = tokens[start].type;
		start++;

		Expression* other = factor();
		res = new Binary(res, other, op);
	}

	return res;
}

Expression* Parser::factor() {
	Expression* res = unary();

	while (check(Divide) || check(Multiply)) {
		TokenType op = tokens[start].type;
		start++;

		Expression* other = unary();
		res = new Binary(res, other, op);
	}

	return res;
}

Expression* Parser::unary() {
	//If the next token is another unary operator
	if (check(Minus) || check(Plus)) {
		TokenType op = tokens[start].type;
		start++;

		Expression* expression = unary();
		Expression* res = new Unary(expression, op);
		return res;
	}

	return call();
}

Expression* Parser::call() {
	Expression* name = primary();
	while (start <= tokens.size() && tokens[start].type == OpenBracket) 
		name = finishCall(name);
	
	return name;
}

Expression* Parser::finishCall(Expression* name) {
	std::vector<Expression*> arguments;
	consume(OpenBracket, "Expected '['");

	if (tokens[start].type != ClosedBracket) {
		arguments.push_back(equality());

		while (check(Comma)) {
			consume(Comma, "Expected ','");
			arguments.push_back(equality());
		}
	}

	TokenType paren = consume(ClosedBracket, "Expected ']'");

	return new FunctionCall(name, paren, arguments);
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

	throw SyntaxError(token.line, "Expected a literal");
}

TokenType Parser::consume(TokenType token, std::string message) {
	if (tokens[start].type != token)
		throw SyntaxError(tokens[start].line, message.c_str());

	TokenType prev = tokens[start].type;
	start++;
	return prev;
}

bool Parser::check(TokenType token) {
	return start <= tokens.size() && tokens[start].type == token;
}