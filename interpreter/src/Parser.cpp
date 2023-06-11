#include "Parser.h"

using namespace parser;

std::string get_token_name(token::TokenType value) {
	std::map<std::string, token::TokenType> tokenMap = {
    {"{", token::TokenType::OPEN},
    {"}", token::TokenType::CLOSE},
    {"when", token::TokenType::WHEN},
    {"do", token::TokenType::DO},
    {"otherwise", token::TokenType::OTHERWISE},
    {"yes", token::TokenType::YES},
    {"no", token::TokenType::NO},
    {"variable", token::TokenType::IDENTI},
    {"text", token::TokenType::TEXT},
    {"number", token::TokenType::NUMBER},
    {"bool", token::TokenType::BOOL},
    {"set", token::TokenType::SET},
    {"to", token::TokenType::TO},
    {"+", token::TokenType::PLUS},
    {"-", token::TokenType::MINUS},
    {"*", token::TokenType::MULTIPLY},
    {"=", token::TokenType::EQUAL},
    {">", token::TokenType::GREATER},
    {"<", token::TokenType::SMALLER},
	{"<end of file>", token::TokenType::END_OF_TOKENS},
	};

    for (const auto& pair : tokenMap) {
        if (pair.second == value) {
            return pair.first;
        }
    }
    return "<unnamed token>"; // Return an empty string if the enum value is not found in the map
}

inline token::Token Parser::next_token() {
	return tokens[index++];
}

void Parser::throw_unexpected_token(token::TokenType type) {
	throw std::invalid_argument(
		"Syntax Error: Unexpected token: \"" + get_token_name(type) + "\"."
	);
}
token::Token Parser::expect_token(vector<token::TokenType> types) {
	token::Token tk = next_token();
	vector<string> names = {};
	bool valid = false;

	for (token::TokenType type: types) {
		string token_name = get_token_name(type);
		names.push_back(token_name);
		
		if (tk.get_type() == type)
			valid = true;
	}

	if (!valid)
		throw_unexpected_token(tk.get_type());
	
	return tk;
}

token::Token Parser::expect_token(token::TokenType type) {
	return expect_token(vector<token::TokenType>{type});
}

Parser::Parser(const string& code) : index { 0 }, tokens { lexer::Lexer(code).lex() }
{
	tokens.push_back(token::TokenType::END_OF_TOKENS);
}

MulExp Parser::parseMulExp() {
	MulExp ret;

	expect_token(token::TokenType::MULTIPLY);
	ret.value = expect_token(token::TokenType::NUMBER);

	return ret;
}

NumExp Parser::parseNumExp() {
	NumExp ret;
	vector<MulExp> muls;

	ret.value = expect_token(token::TokenType::NUMBER);

	while (tokens[index].get_type() == token::TokenType::MULTIPLY)
		muls.push_back(parseMulExp());

	ret.multiplys = muls;
	return ret;
}

PlusExp Parser::parsePlusExp() {
	PlusExp ret;
	token::Token tk = expect_token({token::TokenType::PLUS, token::TokenType::MINUS});
	
	if (tk.get_type() == token::TokenType::PLUS)
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
		throw_unexpected_token(tokens[index].get_type());
	}

	switch (tokens[index].get_type()) {
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
		throw_unexpected_token(tokens[index].get_type());
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
	

	expect_token(token::TokenType::SET);
	ret.identifier = expect_token(token::TokenType::IDENTI);
	expect_token(token::TokenType::TO);
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
		throw_unexpected_token(tokens[index].get_type());
	}

	return ret;
}

BlockExp Parser::parseBlockExp() {
	BlockExp ret;
	vector<Statement> statements;

	expect_token(token::TokenType::OPEN);

	while (tokens[index].get_type() != token::TokenType::CLOSE)
		statements.push_back(parseStatement());

	ret.statements = statements;
	expect_token(token::TokenType::CLOSE);

	return ret;
}

WhenExp Parser::parseWhenExp() {
	WhenExp ret;

	expect_token(token::TokenType::WHEN);
	ret.exp = parseBoolExp();
	expect_token(token::TokenType::DO);
	ret.when_block = parseBlockExp();

	if (tokens[index].get_type() == token::TokenType::OTHERWISE) {
		expect_token(token::TokenType::OTHERWISE);
		expect_token(token::TokenType::DO);
		ret.otherwise = true;
		ret.otherwise_block = parseBlockExp();
	}

	else
		ret.otherwise = false;

	return ret;
}

File Parser::parseFile() {
	File ret;

	while (tokens[index].get_type() != token::TokenType::END_OF_TOKENS)
		ret.code.push_back(parseStatement());

	return ret;
}
