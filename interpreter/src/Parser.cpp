#pragma once
#include "Parser.h"

using namespace parser;

std::string getStringFromEnum(token::TokenType value) {
	map<string, token::token_t> keywords_map = {
	{"{", token::TokenType::OPEN},
	{"}", token::TokenType::CLOSE},
	{"when", token::TokenType::WHEN},
	{"do", token::TokenType::DO},
	{"otherwise", token::TokenType::OTHERWISE},
	{"set", token::TokenType::SET},
	{"to", token::TokenType::TO},
	{"+", token::TokenType::PLUS},
	{"-", token::TokenType::MINUS},
	{"*", token::TokenType::MULTIPLY},
	{"=", token::TokenType::EQUAL},
	{">", token::TokenType::GREATER},
	{"<", token::TokenType::SMALLER},
	{"var", token::TokenType::IDENTI},
};

    for (const auto& pair : keywords_map) {
        if (pair.second == value) {
            return pair.first;
        }
    }
    return "~"; // Return an empty string if the enum value is not found in the map
}

inline token::Token Parser::next_token() {
	return tokens[index++];
}

Parser::Parser(const string& code) : tokens{ lexer::Lexer(code).lex() }, index{ 0 }
{
	tokens.push_back(token::TokenType::END_OF_TOKENS);
}

MulExp Parser::parseMulExp() {
	MulExp ret;

	ret.multiplay = next_token();
	ret.value = next_token();

	return ret;
}

NumExp Parser::parseNumExp() {
	NumExp ret;
	vector<MulExp> muls;

	ret.value = next_token();

	while (tokens[index].get_type() == token::TokenType::MULTIPLY)
		muls.push_back(parseMulExp());

	ret.multiplys = muls;
	return ret;
}

PlusExp Parser::parsePlusExp() {
	PlusExp ret;

	ret.plus = next_token();
	ret.num = parseNumExp();

	return ret;
}

ArithmeticExp Parser::parseArithmeticExp() {
	ArithmeticExp ret;
	vector<PlusExp> pluses;

	ret.num = parseNumExp();

	while (
		tokens[index].get_type() == token::TokenType::PLUS
		|| tokens[index].get_type() == token::TokenType::MINUS
	)
		pluses.push_back(parsePlusExp());

	ret.pluses = pluses;
	return ret;
}

Expression Parser::parseExpression() {
	Expression ret;
	int i = index;

	switch (tokens[index].get_type())
	{
	case token::TokenType::IDENTI:
		ret.identifier = next_token();
		ret.type = Type::IDENTIFIER;
		break;

	case token::TokenType::NUMBER:
		ret.math_exp = parseArithmeticExp();
		ret.type = Type::INTEGER;
		break;

	case token::TokenType::TEXT:
		ret.string = next_token();
		ret.type = Type::STRING;
		break;

	case token::TokenType::BOOL:
		ret.boolean = parseBoolExp();
		ret.type = Type::BOOLEAN;
		break;

	default:
		throw std::invalid_argument("Syntax Error 7");
	}

	switch (tokens[index].get_type()) {

	case token::TokenType::PLUS:
	case token::TokenType::MINUS:
	case token::TokenType::MULTIPLY:
		index = i;
		ret.type = Type::INTEGER;
		ret.math_exp = parseArithmeticExp();
		break;

	case token::TokenType::EQUAL:
	case token::TokenType::GREATER:
	case token::TokenType::SMALLER:
		index = i;
		ret.type = Type::BOOLEAN;
		ret.boolean = parseBoolExp();
		break;

	default:
		break;
	}
	

	return ret;
}

bool is_bool_operator(token::Token op) {
	return (
		op.get_type() == token::TokenType::EQUAL
		|| op.get_type() == token::TokenType::GREATER
		|| op.get_type() == token::TokenType::SMALLER
	);
}		

Operand Parser::parseOperand() {
	Operand ret;
	switch (tokens[index].get_type())
	{
	case token::TokenType::IDENTI:
		ret.identifier = next_token();
		ret.type = Type::IDENTIFIER;
		break;

	case token::TokenType::NUMBER:
		ret.math_exp = parseArithmeticExp();
		ret.type = Type::INTEGER;
		break;

	case token::TokenType::TEXT:
		ret.string = next_token();
		ret.type = Type::STRING;
		break;

	case token::TokenType::BOOL:
		ret.boolean = next_token();
		ret.type = Type::BOOLEAN;
		break;

	default:
		throw std::invalid_argument("Syntax Error 8");
	}

	return ret;
}

BoolExp Parser::parseBoolExp() {
	BoolExp ret;

	ret.left = parseOperand();
	
	if (!is_bool_operator(tokens[index])) {
		ret.type = BoolType::EXPRESSION;
		return ret;
	}

	ret.op = next_token();
	ret.right = parseOperand();
	ret.type = BoolType::QUERY;

	return ret;
}

AssignmentExp Parser::parseAssignmentExp() {
	AssignmentExp ret;
	

	if (next_token().get_type() != token::TokenType::SET)
		throw std::invalid_argument("Syntax Error: Expecting keyword \"set\"");
	
	token::Token tk = next_token();
	if (tk.get_type() != token::TokenType::IDENTI)
		throw std::invalid_argument("Syntax Error: Expecting identifier");
	
	ret.identifier = tk;

	if (next_token().get_type() != token::TokenType::TO)
		throw std::invalid_argument("Syntax Error: Expecting keyword \"to\"");
	
	ret.value = parseExpression();
	ret.type = ret.value.type;
	return ret;
}

Statement Parser::parseStatement() {
	Statement ret;

	switch (tokens[index].get_type())
	{
	case token::TokenType::WHEN:
		ret.type = StatementType::WHEN_TYPE;
		ret.when_statement = parseWhenExp();
		break;

	case token::TokenType::SET:
		ret.type = StatementType::ASSIGNMENT_TYPE;
		ret.assignment_statement = parseAssignmentExp();
		break;

	case token::TokenType::OPEN:
		ret.type = StatementType::BLOCK_TYPE;
		ret.block_statement = parseBlockExp();
		break;

	default:
		throw std::invalid_argument("Syntax Error 9");
		break;
	}

	return ret;
}

BlockExp Parser::parseBlockExp() {
	BlockExp ret;
	vector<Statement> statements;

	ret.open_brackets = next_token();

	while (tokens[index].get_type() != token::TokenType::CLOSE)
		statements.push_back(parseStatement());

	ret.statements = statements;
	ret.close_brackets = next_token();

	return ret;
}

WhenExp Parser::parseWhenExp() {
	WhenExp ret;

	ret.when_token = next_token();
	ret.exp = parseBoolExp();
	ret.do_token = next_token();
	ret.when_block = parseBlockExp();

	if (tokens[index].get_type() == token::TokenType::OTHERWISE) {

		ret.otherwise_token = next_token();
		ret.otherwise_do_token = next_token();
		ret.otherwise_block = parseBlockExp();
	}

	return ret;
}

File Parser::parseFile() {
	File ret;

	while (tokens[index].get_type() != token::TokenType::END_OF_TOKENS)
		ret.code.push_back(parseStatement());

	return ret;
}
