#pragma once
#include <vector>
#include <string>
#include <map>
#include <iostream>

enum TokenType {
	//Identifiers
	Identifier,
	String,
	Number,

	//Operators
	Plus,
	Minus,
	Divide,
	Multiply,
	Dot,
	Increment, 
	Decrement,
	AddAssign,
	SubAssign,
	MultAssign,
	DivAssign,


	//Comparison
	Greater,
	GreaterEqual,
	Less,
	LessEqual,
	EqualCompare,
	NotEqual,

	//Parenthesis
	OpenBracket,
	ClosedBracket,
	OpenParenthesis,
	ClosedParenthesis,
	OpenCurly,
	ClosedCurly,

	//Variables
	Equal,
	Set,
	Null,

	//Keywords
	Return,
	Function,

	//Conditionals
	And,
	Or,
	Xor,
	Not,
	True,
	False,

	//Iterators
	For,
	While,

	//Ifs
	If,
	Else,
	Elif,

	Comma,
	Comment,
	Class,
	PrintStatement,
	Invalid,
	EndOfFile,
};

struct Token {
	std::string value;
	TokenType type;
};

extern std::map<std::string, TokenType> keywords;
extern std::map<TokenType, std::string> type;

class Lexer {
private:
	std::string src;
	int start;

	bool isNumeric(char val);
	bool checkNextCharacter(char val);
	Token createMultiCharacterToken();
	Token createNumericToken();
	Token createStringToken();
	void skipComment();
	Token createToken(TokenType type);
	Token createToken(std::string val, TokenType type);
	Token createToken(char val);

public:
	Lexer(std::string src);
	std::vector<Token> tokenize();
};