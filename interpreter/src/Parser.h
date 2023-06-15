#pragma once
#include <vector>
#include <numeric>

#include "Token.h"
#include "Lexer.h"

using std::vector;

std::string get_token_name(token::TokenType value);

namespace parser
{
	struct Statement;
	struct Expression;
	struct FunctionCall;

	enum class Type {
		IDENTIFIER,
		INTEGER,
		STRING,
		BOOLEAN,
		FUNCTION
	};

	enum class FuncType {
		Action,
		Value,
	};

	enum class OperandType {
		Expression,
		Literal,
	};

	enum class StatementType {
		ASSIGNMENT_TYPE,
		WHEN_TYPE,
		BLOCK_TYPE,
		CALL_TYPE
	};

	enum class NumberType {
		Func,
		Token
	};

	struct NumberExp {
		FunctionCall* func_value;
		token::Token token_value;

		NumberType type;
	};

	struct MulExp {
		NumberExp number;
	};

	struct NumExp {
		NumberExp number;
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

	struct FunctionCall {
		string name;
		FuncType type;
		vector<Expression> params;
	};

	struct Operand {
		token::Token literal_value;
		
		token::Token op;
		Expression* exp_value;

		OperandType type;
	};

	struct BoolExp {
		Operand left;
		vector<Operand> operands;
	};

	struct Expression {
		ArithmeticExp math_exp;
		BoolExp boolean;
		FunctionCall function;

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
		FunctionCall function_statement;

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
		NumberExp parseNumberExp();
		MulExp parseMulExp();
		NumExp parseNumExp();
		PlusExp parsePlusExp();
		ArithmeticExp parseArithmeticExp();
		Expression parseExpression(bool is_operand);
		Operand parseOperand();
		BoolExp parseBoolExp();
		AssignmentExp parseAssignmentExp();
		Statement parseStatement();
		BlockExp parseBlockExp();
		WhenExp parseWhenExp();
		vector<Expression> parseParams();
		FunctionCall parseFunctionCall();

		inline token::Token next_token();
		token::Token expect_token(vector<token::TokenType> types);
		token::Token expect_token(token::TokenType type);
		void throw_unexpected_token(token::TokenType type);

		int index;
		vector<token::Token> tokens;
	};
}
