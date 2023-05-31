#pragma once
#include <vector>
#include "Token.h"
#include "Lexer.h"

using std::vector;

namespace parser
{
	struct Statement;
	struct IfExp;
	struct BlockExp;
	struct BoolExp;
	struct Expression;

	enum class Type {
		IDENTIFIER,
		INTEGER,
		STRING,
		BOOLEAN,
	};

	enum class BoolType {
		QUERY, // if i == j
		EXPRESSION, // if i
	};

	enum class StatementType {
		ASSIGNMENT_TYPE,
		IF_TYPE,
		BLOCK_TYPE,
	};


	struct MulExp {
		token::Token multiplay;
		token::Token value;
	};

	struct NumExp {
		token::Token value;
		vector<MulExp> multiplys;
	};

	struct PlusExp {
		token::Token plus;
		NumExp num;
	};

	struct ArithmeticExp {
		NumExp num;
		vector<PlusExp> pluses;
	};

	struct Operand {
		ArithmeticExp math_exp;
		
		token::Token boolean;
		token::Token identifier;
		token::Token string;

		Type type;
	};

	struct BoolExp {
		Operand left;

		// optional
		token::Token op;
		Operand right;

		BoolType type;
	};

	struct Expression {
		ArithmeticExp math_exp;
		BoolExp boolean;

		token::Token identifier;
		token::Token string;

		Type type;
	};

	struct AssignmentExp {
		token::Token identifier;
		token::Token assignment;

		Expression value;
		Type type;
	};

	struct BlockExp {
		token::Token open_brackets;
		vector<Statement> statements;
		token::Token close_brackets;
	};

	struct IfExp {
		token::Token if_token;
		BoolExp exp;
		BlockExp if_block;

		token::Token else_token;
		BlockExp else_block;
	};

	struct Statement {
		IfExp if_statement;
		AssignmentExp assignment_statement;
		BlockExp block_statement;

		StatementType type;
	};

	struct File {
		vector<Statement> code;
	};

	class Parser {
	public:
		Parser(const string& code);
		inline token::Token next_token();
		File parseFile();
	
	private:
		MulExp parseMulExp();
		NumExp parseNumExp();
		PlusExp parsePlusExp();
		ArithmeticExp parseArithmeticExp();
		Expression parseExpression();
		Operand parseOperand();
		BoolExp parseBoolExp();
		BoolExp parseBoolExp(Operand left);
		AssignmentExp parseAssignmentExp();
		Statement parseStatement();
		BlockExp parseBlockExp();
		IfExp parseIfExp();

		int index;
		vector<token::Token> tokens;
	};
}
