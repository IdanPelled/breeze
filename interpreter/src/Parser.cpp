#include "Parser.h"

using namespace parser;


inline lexer::Token Parser::next_token() {
	return tokens[index++];
}

lexer::Token Parser::expect_token(vector<lexer::TokenType> types) {
	lexer::Token tk = next_token();
	bool valid = false;

	for (lexer::TokenType type: types) {
		if (tk.type == type)
			valid = true;
	}

	if (!valid)
		unexpected_token(tk);
	
	return tk;
}

lexer::Token Parser::expect_token(lexer::TokenType type) {
	return expect_token(vector<lexer::TokenType>{type});
}

Parser::Parser(const string& code) : index { 0 }, tokens { lexer::Lexer().lex(code) }
{
	lexer::Token t;
	t.type = lexer::TokenType::END_OF_TOKENS;
	tokens.push_back(t);
}

NumberExp Parser::parseNumberExp() {
	NumberExp ret;

	if (tokens[index].type == lexer::TokenType::VALUE_FUNCTION) {
		ret.func_value = new FunctionCall(parseFunctionCall());
		ret.type = NumberType::Func;
		return ret;
	}
	
	ret.type = NumberType::Token;
	ret.token_value = expect_token({
		lexer::TokenType::NUMBER_LITERAL,
		lexer::TokenType::VARIABLE,
	});

	return ret;
	
}

MulExp Parser::parseMulExp() {
	MulExp ret;

	lexer::Token tk = expect_token({lexer::TokenType::MULTIPLY, lexer::TokenType::DIVIDE});

	ret.is_multiply = tk.type == lexer::TokenType::MULTIPLY;
	ret.number = parseNumberExp();

	return ret;
}

NumExp Parser::parseNumExp() {
	NumExp ret;
	vector<MulExp> muls;

	ret.number = parseNumberExp();

	while (
		tokens[index].type == lexer::TokenType::MULTIPLY
		|| tokens[index].type == lexer::TokenType::DIVIDE
	)
		muls.push_back(parseMulExp());

	ret.multiplys = muls;
	return ret;
}

PlusExp Parser::parsePlusExp() {
	PlusExp ret;
	lexer::Token tk = expect_token({lexer::TokenType::PLUS, lexer::TokenType::MINUS});
	
	if (tk.type == lexer::TokenType::PLUS)
		ret.is_plus = true;
	else
		ret.is_plus = false;

	ret.num = parseNumExp();

	return ret;
}

ArithmeticExp Parser::parseArithmeticExp() {
	ArithmeticExp ret;
	vector<PlusExp> pluses;

	ret.num = parseNumExp();

	while (
		tokens[index].type == lexer::TokenType::PLUS
		|| tokens[index].type == lexer::TokenType::MINUS
	)
		pluses.push_back(parsePlusExp());

	ret.pluses = pluses;
	return ret;
}

Expression Parser::parseExpression(bool is_operand = false) {
	Expression ret;
	int i = index;
	switch (tokens[index].type)
	{
	case lexer::TokenType::VARIABLE:
		ret.identifier = next_token();
		ret.type = Type::IDENTIFIER;
		break;

	case lexer::TokenType::NUMBER_LITERAL:
		ret.math_exp = parseArithmeticExp();
		ret.type = Type::INTEGER;
		break;

	case lexer::TokenType::TEXT_LITERAL:
		ret.string = next_token();
		ret.type = Type::STRING;
		break;

	case lexer::TokenType::BOOLEAN_LITERAL:
		ret.boolean = parseBoolExp();
		ret.type = Type::BOOLEAN;
		break;

	case lexer::TokenType::VALUE_FUNCTION:
		ret.function = parseFunctionCall();
		ret.type = Type::FUNCTION;
		break;

	default:
		unexpected_token(tokens[index]);
	}

	switch (tokens[index].type) {
	case lexer::TokenType::PLUS:
	case lexer::TokenType::MINUS:
	case lexer::TokenType::MULTIPLY:
	case lexer::TokenType::DIVIDE:
		index = i;
		ret.type = Type::INTEGER;
		ret.math_exp = parseArithmeticExp();
		break;

	case lexer::TokenType::EQUAL:
	case lexer::TokenType::GREATER:
	case lexer::TokenType::SMALLER:
		if (!is_operand) {
			index = i;
			ret.type = Type::BOOLEAN;
			ret.boolean = parseBoolExp();
		}
		break;

	default:
		break;
	}
	return ret;
}

bool is_bool_operator(lexer::Token op) {
	return (
		op.type == lexer::TokenType::EQUAL
		|| op.type == lexer::TokenType::GREATER
		|| op.type == lexer::TokenType::SMALLER
	);
}  

Operand Parser::parseOperand() {
	Operand ret;
	int i = index;
	lexer::Token tk = next_token();
	if (
		tk.type == lexer::TokenType::BOOLEAN_LITERAL
		&& !is_bool_operator(tokens[index])
	)
	{
		ret.type = OperandType::Literal;
		ret.literal_value = tk;
		return ret;
	}

	ret.type = OperandType::Expression;
	index = i;
	ret.exp_value = new Expression(parseExpression(true));
	return ret;	

}



BoolExp Parser::parseBoolExp() {
	BoolExp ret;
	Operand tmp;
	ret.left = parseOperand();

	while (is_bool_operator(tokens[index])) {
		lexer::Token op = expect_token({
			lexer::TokenType::EQUAL, 
			lexer::TokenType::GREATER, 
			lexer::TokenType::SMALLER
		});
		tmp.op = op;
		tmp.exp_value = new Expression(parseExpression(true));
		tmp.type = OperandType::Expression;
		ret.operands.push_back(tmp);
	}

	return ret;
}

AssignmentExp Parser::parseAssignmentExp() {
	AssignmentExp ret;
	
	expect_token(lexer::TokenType::SET);
	ret.identifier = expect_token(lexer::TokenType::VARIABLE);
	expect_token(lexer::TokenType::TO);	
	ret.value = parseExpression();
	ret.type = ret.value.type;
	return ret;
}

Statement Parser::parseStatement() {
	Statement ret;

	switch (tokens[index].type)
	{
	case lexer::TokenType::WHEN:
		ret.type = StatementType::WHEN_TYPE;
		ret.when_statement = parseWhenExp();
		break;

	case lexer::TokenType::LOOP:
		ret.type = StatementType::LOOP_TYPE;
		ret.loop_statement = parseLoopExp();
		break;
	case lexer::TokenType::WHILE:
		ret.type = StatementType::WHILE_TYPE;
		ret.while_statement = parseWhileExp();
		break;
	case lexer::TokenType::SET:
		ret.type = StatementType::ASSIGNMENT_TYPE;
		ret.assignment_statement = parseAssignmentExp();
		break;

	case lexer::TokenType::OPEN:
		ret.type = StatementType::BLOCK_TYPE;
		ret.block_statement = parseBlockExp();
		break;
	
	case lexer::TokenType::ACTION_FUNCTION:
		ret.type = StatementType::CALL_TYPE;
		ret.function_statement = parseFunctionCall();
		break;
	
	case lexer::TokenType::VALUE_FUNCTION:
		throw std::invalid_argument("Value function must save the return value");

	default:
		unexpected_token(tokens[index]);
	}

	return ret;
}

BlockExp Parser::parseBlockExp() {
	BlockExp ret;
	vector<Statement> statements;

	expect_token(lexer::TokenType::OPEN);

	while (tokens[index].type != lexer::TokenType::CLOSE)
		statements.push_back(parseStatement());

	ret.statements = statements;
	expect_token(lexer::TokenType::CLOSE);

	return ret;
}

WhileExp Parser::parseWhileExp() {
	WhileExp ret;
	expect_token(lexer::TokenType::WHILE);
	ret.exp = parseBoolExp();
	expect_token(lexer::TokenType::DO);
	ret.while_block = parseBlockExp();
}

WhenExp Parser::parseWhenExp() {
	WhenExp ret;

	expect_token(lexer::TokenType::WHEN);
	ret.exp = parseBoolExp();
	expect_token(lexer::TokenType::DO);
	ret.when_block = parseBlockExp();

	if (tokens[index].type == lexer::TokenType::OTHERWISE) {
		expect_token(lexer::TokenType::OTHERWISE);
		expect_token(lexer::TokenType::DO);
		ret.otherwise = true;
		ret.otherwise_block = parseBlockExp();
	}

	else
		ret.otherwise = false;

	return ret;
}

LoopExp Parser::parseLoopExp() {
	LoopExp ret;
	
	expect_token(lexer::TokenType::LOOP);
	ret.times = parseArithmeticExp();
	expect_token(lexer::TokenType::TIMES);
	ret.loop_block = parseBlockExp();

	return ret;
}

vector<Expression> Parser::parseParams() {
	vector<Expression> ret;
	expect_token(lexer::TokenType::CALL_OPEN);

	if (tokens[index].type != lexer::TokenType::CALL_CLOSE)
		ret.push_back(parseExpression());
	
	while (tokens[index].type != lexer::TokenType::CALL_CLOSE) {
		expect_token(lexer::TokenType::COMMA);
		ret.push_back(parseExpression());
	}

	expect_token(lexer::TokenType::CALL_CLOSE);
	return ret;
}

FunctionCall Parser::parseFunctionCall() {
	FunctionCall ret;

	lexer::Token function = expect_token(
		{
			lexer::TokenType::VALUE_FUNCTION,
			lexer::TokenType::ACTION_FUNCTION
		}
	);
	
	switch(function.type) {
	case lexer::TokenType::VALUE_FUNCTION:
		ret.type = lexer::FunctionType::Value;
		break;

	case lexer::TokenType::ACTION_FUNCTION:
		ret.type = lexer::FunctionType::Action;
		break;
	
	default:
		throw std::invalid_argument("Syntax error 1");
	}


	
	ret.params = parseParams();
	ret.name = function.value;
	return ret;

}

File Parser::parseFile() {
	File ret;

	while (tokens[index].type != lexer::TokenType::END_OF_TOKENS)
		ret.code.push_back(parseStatement());

	return ret;
}
