#pragma once
#include <vector>
#include <numeric>

#include "Lexer.h"

using std::vector;

std::string get_token_name(lexer::TokenType value);

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

	enum class OperandType {
		Expression,
		Literal,
	};

	enum class StatementType {
		ASSIGNMENT_TYPE,
		WHEN_TYPE,
		LOOP_TYPE,
		BLOCK_TYPE,
		CALL_TYPE
	};

	enum class NumberType {
		Func,
		Token
	};

	struct Variable {
		string name;

		int number_val;
		bool boolean_val;
		string text_val;

		lexer::VarType type;
	};

	struct ReturnType {
		string text;
		int number;
		bool boolean;

		lexer::VarType type;
	};

	struct NumberExp {
		FunctionCall* func_value;
		lexer::Token token_value;

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
		lexer::FunctionType type;
		vector<Expression> params;
	};

	struct Operand {
		lexer::Token literal_value;
		
		lexer::Token op;
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

		lexer::Token identifier;
		lexer::Token string;

		Type type;
	};

	struct AssignmentExp {
		lexer::Token identifier;

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

	struct LoopExp {
		ArithmeticExp times;
		BlockExp loop_block;
	};


	struct Statement {
		WhenExp when_statement;
		LoopExp loop_statement;
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
		LoopExp parseLoopExp();
		vector<Expression> parseParams();
		FunctionCall parseFunctionCall();

		inline lexer::Token next_token();
		lexer::Token expect_token(vector<lexer::TokenType> types);
		lexer::Token expect_token(lexer::TokenType type);

		int index;
		vector<lexer::Token> tokens;
	};
}
