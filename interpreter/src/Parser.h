#pragma once
#include <vector>
#include "Token.h"
#include "Lexer.h"

using std::vector;

namespace parser
{
	struct Statement;
	struct WhenExp;
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
		QUERY, // when i == j
		EXPRESSION, // when i
	};

	enum class StatementType {
		ASSIGNMENT_TYPE,
		WHEN_TYPE,
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

	struct WhenExp {
		token::Token when_token;
		BoolExp exp;
		BlockExp when_block;

		token::Token otherwise_token;
		BlockExp otherwise_block;
	};

	struct Statement {
		WhenExp when_statement;
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
		WhenExp parseWhenExp();

		int index;
		vector<token::Token> tokens;
	};
}
