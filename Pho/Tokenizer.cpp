#pragma once
#include "Tokenizer.h"

std::map<std::string, TokenType> keywords = {
	{"ret", Return},
	{"set", Set},

	{"and", And},
	{"or", Or},
	{"xor", Xor},
	{"not", Not},
	{"true", True},
	{"false", False},

	{"for", For},
	{"while", While},

	{"if", If},
	{"else", Else},
	{"elif", Elif},
	{"fun", Function},

	{"class", Class},
	{"null", Null},
	{"print", PrintStatement}
};


std::map<TokenType, std::string> types = {
	{Identifier, "Identifier"},
	{String, "String"},
	{Number, "Number"},

	{Plus, "Plus"},
	{Minus, "Minus"},
	{Multiply, "Multiply"},
	{Divide, "Divide"},

	{Less, "Less"},
	{LessEqual, "LessEqual"},
	{Greater, "Greater"},
	{GreaterEqual, "GreaterEqual"},
	{EqualCompare, "EqualCompare"},

	{And, "And"},
	{Or, "Or"},
	{Xor, "Xor"},
	{Not, "Not"},

	{OpenBracket, "OpenBracket"},
	{ClosedBracket, "ClosedBracket"},
	{OpenParenthesis, "OpenParenthesis"},
	{ClosedParenthesis, "ClosedParenthesis"},
	{OpenCurly, "OpenCurly"},
	{ClosedCurly, "ClosedCurly"},

	{Equal, "Equal"},

	{Return, "Return"},
	{Set, "Set"},
	{Invalid, "Invalid"},

	{For, "For"},
	{While, "While"},

	{If, "If"},
	{Else, "Else"},
	{Elif, "Elif"},

	{Function, "Function"},
	{Comment, "Comment"},
	{Comma, "Comma"},
};

Lexer::Lexer(std::string src) : src(src), start(0), line(1) {}

bool Lexer::isNumeric(char val) {
	return val >= 48 && val <= 57;
}

bool Lexer::checkNextCharacter(char val) {
	if (start + 1 < src.length() && src[start + 1] == val) {
		start++;
		return true;
	}

	return false;
}


//Creates a multicharacter token
Token Lexer::createMultiCharacterToken() {
	int end = start + 1;
	while (end < src.length() && isalpha(src[end]))
		end++;

	std::string word = src.substr(start, end - start);
	start = end;

	//If in keywords 
	if (keywords.count(word))
		return Token{word, keywords[word]};

	return Token{ word, Identifier, line};
}

//Creates a numerical token type
Token Lexer::createNumericToken() {
	int end = start + 1;
	while (end < src.length() && isNumeric(src[end]))
		end++;

	std::string word = src.substr(start, end - start);
	start = end;

	return Token{ word, Number, line};
}

Token Lexer::createStringToken() {
	int end = start + 1;
	while (end < src.length() && src[end] != '"')
		end++;

	std::string word = src.substr(start + 1, end - start - 1);
	start = end + 1;

	return Token{ word, String, line};
}

void Lexer::skipComment() {
	int end = start + 1;
	while (end < src.length() && src[end] != '\n')
		end++;

	start = end + 1;
}

//Creates single character token
Token Lexer::createToken(TokenType type) {
	std::string val{ src[start++] };
	return Token{val, type, line};
}

//Creates single character token
Token Lexer::createToken(std::string val, TokenType type) {
	start++;
	return Token{val, type, line};
}

Token Lexer::createToken(char val) {
	//Check for multi-character tokens 
	if (isalpha(val))
		return createMultiCharacterToken();


	//Check for numeric tokens
	if (isNumeric(val))
		return createNumericToken();


	//Check for non character tokens
	switch (val) {
	case '+':
		if (checkNextCharacter('+'))
			return createToken("++", Increment);
		
		else if (checkNextCharacter('='))
			return createToken("+=", AddAssign);
		
		return createToken(Plus);
			
	case '*':
		return checkNextCharacter('=') ? createToken("*=", MultAssign) : createToken(Multiply);

	case '-':
		/*if (checkNextCharacter('-'))
			return createToken("--", Decrement);*/

		if (checkNextCharacter('='))
			return createToken("-=", SubAssign);

		return createToken(Minus);

	case '.':
		return createToken(Dot);


	case '/':
		if (checkNextCharacter('='))
			return createToken("/=", DivAssign);

		return createToken(Divide);

	case ',':
		return createToken(Comma);

	case '[':
		return createToken(OpenBracket);

	case ']':
		return createToken(ClosedBracket);

	case '(':
		return createToken(OpenParenthesis);

	case ')':
		return createToken(ClosedParenthesis);

	case '{':
		return createToken(OpenCurly);

	case '}':
		return createToken(ClosedCurly);

	case '=':
		return checkNextCharacter('=') ? createToken("==", EqualCompare) : createToken("=", Equal);


	case '<':
		return checkNextCharacter('=') ? createToken("<=", LessEqual) : createToken("<", Less);

	case '>':
		return checkNextCharacter('=') ? createToken(">=", GreaterEqual) : createToken(">", Greater);

	case '"':
		return createStringToken();

	case '!':
		return checkNextCharacter('=') ? createToken("!=", NotEqual) : createToken("!", Not);

	case '&':
		if (checkNextCharacter('&'))
			return createToken("&&", And);

	case '|':
		if (checkNextCharacter('|'))
			return createToken("||", Or);

	case '^':
		return createToken(Xor);

	}

	return Token{ "", Invalid };
}

std::vector<Token> Lexer::tokenize() {
	std::vector<Token> tokens;

	while (start < src.length()) {
		char val = src[start];

		if (val == '/' && checkNextCharacter('/')) {
			skipComment();
			continue;
		}

		if (val == '\n')
			line++;

		if (val != ' ' && val != '\n' && val != '\r' && val != '\t') {
			Token current = createToken(val);
			tokens.push_back(current);
			continue;
		}

		start++;
	}

	tokens.push_back(createToken("EOF", EndOfFile));

	return tokens;
}