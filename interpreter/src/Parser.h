#pragma once
#include <vector>
#include <numeric>
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
		token::Token value;
	};

	struct NumExp {
		token::Token value;
		vector<MulExp> multiplys;
	};

	struct PlusExp {
		NumExp num;

		// plus pr minus
		bool is_plus;
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

		Expression value;
		Type type;
	};

	struct BlockExp {
		vector<Statement> statements;
	};

	struct WhenExp {
		BoolExp exp;
		BlockExp when_block;

		bool otherwise;
		// optional
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

		inline token::Token next_token();
		token::Token expect_token(vector<token::TokenType> types);
		token::Token expect_token(token::TokenType type);
		void throw_unexpected_token(token::TokenType type);

		int index;
		vector<token::Token> tokens;
	};
}
