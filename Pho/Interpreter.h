#include "Environment.h"
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
};