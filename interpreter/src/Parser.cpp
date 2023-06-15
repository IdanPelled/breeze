#include "Parser.h"

using namespace parser;

std::string get_token_name(token::TokenType value) {
	std::map<std::string, token::TokenType> tokenMap = {
		{"{", token::TokenType::OPEN},
		{"}", token::TokenType::CLOSE},
		{"(", token::TokenType::F_OPEN},
		{")", token::TokenType::F_CLOSE},
		{"when", token::TokenType::WHEN},
		{"do", token::TokenType::DO},
		{"otherwise", token::TokenType::OTHERWISE},
		{"loop", token::TokenType::LOOP},
		{"times", token::TokenType::TIMES},
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

NumberExp Parser::parseNumberExp() {
	NumberExp ret;

	if (tokens[index].get_type() == token::TokenType::VALUE_FUNCTION) {
		ret.func_value = new FunctionCall(parseFunctionCall());
		ret.type = NumberType::Func;
	}
	
	else {
		ret.type = NumberType::Token;
		ret.token_value = expect_token({
			token::TokenType::NUMBER,
			token::TokenType::IDENTI,
		});  
	}

	return ret;
}

MulExp Parser::parseMulExp() {
	MulExp ret;

	expect_token(token::TokenType::MULTIPLY);
	ret.number = parseNumberExp();

	return ret;
}

NumExp Parser::parseNumExp() {
	NumExp ret;
	vector<MulExp> muls;

	ret.number = parseNumberExp();

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

Expression Parser::parseExpression(bool is_operand = false) {
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

	case token::TokenType::VALUE_FUNCTION:
		ret.function = parseFunctionCall();
		ret.type = Type::FUNCTION;
		break;

	default:
		throw_unexpected_token(tokens[index].get_type());
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

bool is_bool_operator(token::Token op) {
	return (
		op.get_type() == token::TokenType::EQUAL
		|| op.get_type() == token::TokenType::GREATER
		|| op.get_type() == token::TokenType::SMALLER
	);
}  

Operand Parser::parseOperand() {
	Operand ret;
	int i = index;
	token::Token tk = next_token();
	if (
		tk.get_type() == token::TokenType::BOOL
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
		token::Token op = expect_token({
			token::TokenType::EQUAL, 
			token::TokenType::GREATER, 
			token::TokenType::SMALLER
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

	case token::TokenType::LOOP:
		ret.type = StatementType::LOOP_TYPE;
		ret.loop_statement = parseLoopExp();
		break;
	case token::TokenType::SET:
		ret.type = StatementType::ASSIGNMENT_TYPE;
		ret.assignment_statement = parseAssignmentExp();
		break;

	case token::TokenType::OPEN:
		ret.type = StatementType::BLOCK_TYPE;
		ret.block_statement = parseBlockExp();
		break;
	
	case token::TokenType::ACTION_FUNCTION:
		ret.type = StatementType::CALL_TYPE;
		ret.function_statement = parseFunctionCall();
		break;
	
	case token::TokenType::VALUE_FUNCTION:
		throw std::invalid_argument("Value function must save the return value");

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

LoopExp Parser::parseLoopExp() {
	LoopExp ret;
	
	expect_token(token::TokenType::LOOP);
	ret.times = parseArithmeticExp();
	expect_token(token::TokenType::TIMES);
	ret.loop_block = parseBlockExp();

	return ret;
}

vector<Expression> Parser::parseParams() {
	vector<Expression> ret;
	expect_token(token::TokenType::F_OPEN);

	if (tokens[index].get_type() != token::TokenType::F_CLOSE)
		ret.push_back(parseExpression());
	
	while (tokens[index].get_type() != token::TokenType::F_CLOSE) {
		expect_token(token::TokenType::COMMA);
		ret.push_back(parseExpression());
	}

	expect_token(token::TokenType::F_CLOSE);
	return ret;
}

FunctionCall Parser::parseFunctionCall() {
	FunctionCall ret;

	token::Token function = expect_token(
		{
			token::TokenType::VALUE_FUNCTION,
			token::TokenType::ACTION_FUNCTION
		}
	);
	
	switch(function.get_type()) {
	case token::TokenType::VALUE_FUNCTION:
		ret.type = FuncType::Value;
		break;

	case token::TokenType::ACTION_FUNCTION:
		ret.type = FuncType::Action;
		break;
	
	default:
		throw std::invalid_argument("Syntax error 1");
	}


	
	ret.params = parseParams();
	ret.name = function.get_data("name");
	return ret;

}

File Parser::parseFile() {
	File ret;

	while (tokens[index].get_type() != token::TokenType::END_OF_TOKENS)
		ret.code.push_back(parseStatement());

	return ret;
}
