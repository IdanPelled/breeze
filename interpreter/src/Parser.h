#pragma once
#include <vector>
#include <numeric>

#include "Lexer.h"

using std::vector;

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

		// multiply pr divide
		bool is_multiply;
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

		/**
		 * Constructs a Parser object with the given code.
		 *
		 * This constructor initializes a Parser object by lexing the provided code
		 * using the lexer::Lexer class. It also adds an "END_OF_TOKENS" token to the
		 * list of tokens to mark the end of the token stream.
		 *
		 * @param code The code to be parsed.
		 */
		Parser(const string& code);


		/**
		 * Parses a file from the tokens.
		 *
		 * This function constructs a File object by parsing the tokens and creating a list
		 * of statements within the file. It continues parsing until it encounters the
		 * "END_OF_TOKENS" token.
		 *
		 * @return The parsed File object.
		 */
		File parseFile();
	
	private:

		/**
		 * Parses a number expression from the tokens.
		 * 
		 * This function examines the tokens and constructs a NumberExp object
		 * representing a number expression. The number can either be a token value
		 * or a function call.
		 * 
		 * @return The parsed NumberExp object.
		 */
		NumberExp parseNumberExp();


		/**
		 * Parses a multiplication expression from the tokens.
		 * 
		 * This function expects the next token to be a multiplication operator (*)
		 * and constructs a MulExp object representing a multiplication expression
		 * with a nested number expression.
		 * 
		 * @return The parsed MulExp object.
		 */
		MulExp parseMulExp();


		/**
		 * Parses a number expression with potential multiplication from the tokens.
		 * 
		 * This function constructs a NumExp object representing a number expression
		 * that may include multiplication operations. It recursively calls the
		 * parseMulExp() function if additional multiplications are detected.
		 * 
		 * @return The parsed NumExp object.
		 */
		NumExp parseNumExp();


		/**
		 * Parses an addition or subtraction expression from the tokens.
		 * 
		 * This function expects the next token to be either a plus (+) or a minus (-)
		 * operator. It constructs a PlusExp object representing an addition or
		 * subtraction expression with a nested number expression.
		 * 
		 * @return The parsed PlusExp object.
		 */
		PlusExp parsePlusExp();


		/**
		 * Parses an arithmetic expression with operator priority from the tokens.
		 *
		 * This function constructs an ArithmeticExp object representing an arithmetic
		 * expression with operator priority. It handles addition and subtraction operations
		 * and ensures that multiplication operations are evaluated first. The resulting
		 * expression is represented as a list of PlusExp objects, where each PlusExp object
		 * can contain a list of MulExp objects.
		 *
		 * @return The parsed ArithmeticExp object.
		 */
		ArithmeticExp parseArithmeticExp();


		/**
		 * Parses an expression.
		 *
		 * This function parses an expression, which can be a variable, number literal, text literal,
		 * boolean literal, value function, or an arithmetic expression. It returns an Expression object
		 * representing the parsed expression.
		 *
		 * @param is_operand (optional) - Specifies whether the expression is being parsed as an operand.
		 *                                Defaults to false.
		 * @return Expression - The parsed expression object.
		 */
		Expression parseExpression(bool is_operand);


		/**
		 * Parses an operand.
		 *
		 * This function parses an operand, which can be either a boolean literal or an expression.
		 * It returns an Operand object representing the parsed operand.
		 *
		 * @return Operand - The parsed operand object.
		 */
		Operand parseOperand();


		/**
		 * Parses a boolean expression.
		 *
		 * This function parses a boolean expression, which consists of one or more operands
		 * separated by boolean operators such as 'EQUAL', 'GREATER', or 'SMALLER'. It returns
		 * a BoolExp object representing the parsed boolean expression.
		 *
		 * @return BoolExp - The parsed boolean expression object.
		 */
		BoolExp parseBoolExp();


		/**
		 * Parses an assignment expression.
		 *
		 * This function parses an assignment expression, which consists of the 'SET' keyword,
		 * followed by an identifier, the 'TO' keyword, and an expression representing the value
		 * to be assigned. It returns an AssignmentExp object representing the parsed assignment.
		 *
		 * @return AssignmentExp - The parsed assignment expression object.
		 */
		AssignmentExp parseAssignmentExp();


		/**
		 * Parses a statement.
		 *
		 * This function parses a statement based on the token type and returns a Statement object
		 * representing the parsed statement.
		 *
		 * @return Statement - The parsed statement object.
		 * @throws std::invalid_argument - If a value function is encountered without saving the return value.
		 */
		Statement parseStatement();


		/**
		 * Parses a block expression.
		 *
		 * This function parses a block expression, which is a sequence of statements enclosed in
		 * opening and closing tokens. It returns a BlockExp object containing the parsed statements.
		 *
		 * @return BlockExp - The parsed block expression object.
		 */
		BlockExp parseBlockExp();


		/**
		 * Parses a 'when' expression.
		 *
		 * This function parses a 'when' expression, which consists of a boolean expression,
		 * followed by a 'do' block. It can optionally have an 'otherwise' block as well.
		 *
		 * @return WhenExp - The parsed 'when' expression object.
		 */
		WhenExp parseWhenExp();


        /**
         * Parses a loop expression from the input code.
         *
         * @return The parsed LoopExp object representing the loop expression.
         */
		LoopExp parseLoopExp();


        /**
         * Parses the parameters of a function call from the input code.
         *
         * @return The vector of parsed expressions representing the parameters.
         */
		vector<Expression> parseParams();


        /**
         * Parses a function call from the input code.
         *
         * @return The parsed FunctionCall object representing the function call.
         * @throws std::invalid_argument if a syntax error occurs during parsing.
         */
		FunctionCall parseFunctionCall();


		/**
         * Retrieves the next token from the token stream and advances the index.
         *
         * @return The next token in the token stream.
         */
		inline lexer::Token next_token();


		/**
		 * Expects the next token to be one of the specified types and returns it.
		 *
		 * @param types [in] A vector of expected token types.
		 * @return The next token if it matches one of the expected types.
		 * @throws std::runtime_error if the next token does not match any of the expected types.
		 */
		lexer::Token expect_token(vector<lexer::TokenType> types);


		/**
		 * Expects the next token to be of the specified type and returns it.
		 *
		 * @param type [in] The expected token type.
		 * @return The next token if it matches the expected type.
		 * @throws std::runtime_error if the next token does not match the expected type.
		 */
		lexer::Token expect_token(lexer::TokenType type);

		int index;
		vector<lexer::Token> tokens;
	};
}

/**
 * Retrieves the name of a lexer token type based on its enum value.
 * 
 * @param value The lexer token type enum value.
 * @return The name of the token type as a string.
 * @note If the enum value is not found in the map, an empty string ("<unnamed token>") is returned.
 */
std::string get_token_name(lexer::TokenType value);