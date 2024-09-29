#include "Environment.h"
#include "Error.h"
#include "Visitor.h"

class Interpreter : public Visitor {
	public:
		Environment* environment;
		Interpreter();

		TYPE visitBinary(Binary* e) override;
		TYPE visitUnary(Unary* e) override;
		TYPE visitLiteral(Literal* e) override;
		TYPE visitGrouping(Grouping* e) override;
		TYPE visitPrint(Print* e) override;
		TYPE visitVariableDeclaration(VariableDeclaration* e) override;
		TYPE visitVariable(Variable* e) override;
		TYPE visitVariableAssign(VariableAssign* e) override;
		TYPE visitBlock(Block* e) override;
		TYPE visitIfStatement(IfStatement* e) override;
		TYPE visitWhileLoop(WhileLoop* e) override;
		TYPE visitFunctionCall(FunctionCall* e) override;
		TYPE visitFunctionDeclaration(FunctionDeclaration* e) override;
		TYPE visitReturnStatement(ReturnStatement* e) override;
};