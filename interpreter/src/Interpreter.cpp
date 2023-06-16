#include "Interpreter.h"

using namespace interpreter;


Interpreter::Interpreter(const string& data)
	: code { parser::Parser(data).parseFile() }
{
}

parser::Variable& Interpreter::get_var(string name) {
	try {
		return vars.at(name);
	}
	
	catch (const std::out_of_range& e) {
        throw std::invalid_argument("Key error: \"" + name + "\"");
    }
	
}

int Interpreter::interprerNumberExp(parser::NumberExp exp) {
	parser::ReturnType ret;
	parser::Variable var;

	switch (exp.type)
	{
	case parser::NumberType::Func:
		ret = interprerValueFunctionCall(*(exp.func_value));
		if (ret.type != lexer::VarType::Number)
			throw std::invalid_argument(
				"Runtime error: Arithmetic operation must be on numbers, but Value function returned a non-numbers value"
			);

		return ret.number;
	
	case parser::NumberType::Token:
		switch (exp.token_value.type)
		{
		case lexer::TokenType::VARIABLE:
			var = get_var(exp.token_value.value);
			
			if (var.type != lexer::VarType::Number)
				throw std::invalid_argument("Arithmetic expression can use only variable of type number");
			
			return var.number_val;

		case lexer::TokenType::NUMBER_LITERAL:
			return stoi(exp.token_value.value);
		
		default:
			throw std::invalid_argument("Syntax Error: invalid token");
		}

	default:
		throw std::invalid_argument("Syntax Error: invalid token");
	}
}

int Interpreter::interprerMulExp(parser::MulExp exp) {
	return interprerNumberExp(exp.number);
}

int Interpreter::interprerPlusExp(parser::PlusExp exp) {
	return interprerNumExp(exp.num);

}


int Interpreter::interprerNumExp(parser::NumExp exp) {
	int ret = interprerNumberExp(exp.number);

	for (parser::MulExp mul : exp.multiplys)
		ret *= interprerMulExp(mul);

	return ret;
}

int Interpreter::interprerArithmeticExp(parser::ArithmeticExp exp)
{
	int tmp;
	int ret = interprerNumExp(exp.num);

	for (parser::PlusExp plus : exp.pluses) {
		tmp = interprerPlusExp(plus);
		
		if (plus.is_plus)
			ret += tmp;

		else
			ret -= tmp;
	}

	return ret;
}

parser::ReturnType Interpreter::interprerExpression(parser::Expression exp) {
	parser::ReturnType ret;
	parser::Variable var;

	switch (exp.type)
	{
	case parser::Type::IDENTIFIER:
		var = get_var(exp.identifier.value);

		switch (var.type)
		{
		case lexer::VarType::Number:
			ret.number = var.number_val;
			break;

		case lexer::VarType::Boolean:
			ret.boolean = var.boolean_val;
			break;

		case lexer::VarType::Text:
			ret.text = var.text_val;
			break;
		}
		ret.type = var.type;
		break;
	
	case parser::Type::BOOLEAN:
		ret.type = lexer::VarType::Boolean;

		if(exp.boolean.operands.size() == 0)
			ret.boolean = interprerOperand(exp.boolean.left).boolean;

		ret.boolean = interprerBoolExp(exp.boolean);
		break;

	case parser::Type::STRING:
		ret.text = exp.string.value;
		ret.type = lexer::VarType::Text;
		break;

	case parser::Type::INTEGER:
		ret.number = interprerArithmeticExp(exp.math_exp);
		ret.type = lexer::VarType::Number;
		break;
	
	case parser::Type::FUNCTION:
		ret = interprerValueFunctionCall(exp.function);
		break;
	}

	return ret;
}

bool Interpreter::evaluate_bool(string s) {
	if (s.compare("yes") == 0)
		return true;

	if (s.compare("no") == 0)
		return false;

	throw std::invalid_argument("invalid bool argument");
}


bool evaluate_equal(parser::ReturnType left, parser::ReturnType right) {
	if (left.type != right.type)
		throw std::invalid_argument("Value error: Cant compare diffrent types");

	switch (left.type)
	{
	case lexer::VarType::Boolean:
		return left.boolean == right.boolean;

	case lexer::VarType::Text:
		return left.text.compare(right.text) == 0;

	case lexer::VarType::Number:
		return left.number == right.number;
	
	default:
		throw std::invalid_argument("Invalid type");
	}
}

bool evaluate_grater(parser::ReturnType left, parser::ReturnType right) {
	if (left.type != right.type)
		throw std::invalid_argument("Value error: Cant compare diffrent types");

	switch (left.type)
	{
	case lexer::VarType::Boolean:
		throw std::invalid_argument("Cant use operator < on type boolean");

	case lexer::VarType::Text:
		throw std::invalid_argument("Cant use operator < on type string");

	case lexer::VarType::Number:
		return left.number > right.number;

	default:
		throw std::invalid_argument("Invalid type");
	}
}

bool evaluate_smaller(parser::ReturnType left, parser::ReturnType right) {
	if (left.type != right.type)
		throw std::invalid_argument("Value error: Cant compare diffrent types");

	switch (left.type)
	{
	case lexer::VarType::Boolean:
		throw std::invalid_argument("Cant use operator > on type boolean");

	case lexer::VarType::Text:
		throw std::invalid_argument("Cant use operator > on type string");

	case lexer::VarType::Number:
		return left.number < right.number;

	default:
		throw std::invalid_argument("Invalid type");
	}
}

parser::ReturnType Interpreter::interprerOperand(parser::Operand exp) {
	parser::ReturnType ret;
	
	switch (exp.type)
	{
	case parser::OperandType::Expression:
		return interprerExpression(*exp.exp_value);
	
	case parser::OperandType::Literal:
		ret.type = lexer::VarType::Boolean;
		ret.boolean = evaluate_bool(exp.literal_value.value);
		break;
	}

	return ret;
}


bool Interpreter::interprerBoolExp(parser::BoolExp exp) {
	parser::ReturnType left = interprerOperand(exp.left);
	parser::ReturnType right, tmp;
	bool ret = false;

	for(auto operand : exp.operands) {
		right = interprerOperand(operand);
		
		switch (operand.op.type)
		{
		case lexer::TokenType::EQUAL:
			ret = evaluate_equal(left, right);
			break;

		case lexer::TokenType::GREATER:
			ret = evaluate_grater(left, right);
			break;

		case lexer::TokenType::SMALLER:
			ret = evaluate_smaller(left, right);
			break;

		default:
			throw std::invalid_argument("Syntax error");
		}

		tmp.type = lexer::VarType::Boolean;
		tmp.boolean = ret;
		left = tmp;

	}
	ret = left.boolean;
	return ret;
}

map<string, lexer::VarType> get_function(const string& name) {
	for (auto type: functions) {
		
		for (auto pair: type.second) {			
			if (pair.first == name)
				return pair.second;
		}
	}

	throw std::invalid_argument("Function does not exist");
}

void Interpreter::interprerAssignment(parser::AssignmentExp exp) {
	parser::Variable var;
	var.name = exp.identifier.value;
	switch (exp.type)
	{
	case parser::Type::INTEGER:
		var.number_val = interprerArithmeticExp(exp.value.math_exp);
		var.type = lexer::VarType::Number;
		break;

	case parser::Type::STRING:
		var.text_val = exp.value.string.value;
		var.type = lexer::VarType::Text;
		break;

	case parser::Type::BOOLEAN:
		var.boolean_val = interprerBoolExp(exp.value.boolean);
		var.type = lexer::VarType::Boolean;
		break;

	case parser::Type::IDENTIFIER:
		switch (get_var(exp.value.identifier.value).type)
		{
		case lexer::VarType::Number:
			var.number_val = interprerArithmeticExp(exp.value.math_exp);
			var.type = lexer::VarType::Number;
			break;

		case lexer::VarType::Text:
			var.text_val = exp.value.string.value;
			var.type = lexer::VarType::Text;
			break;

		case lexer::VarType::Boolean:
			var.boolean_val = interprerBoolExp(exp.value.boolean);
			var.type = lexer::VarType::Boolean;
			break;

		default:
			break;
		}
		break;

	case parser::Type::FUNCTION:
		switch (get_function(exp.value.function.name)["return"])
		{
		case lexer::VarType::Boolean:
			var.number_val = interprerValueFunctionCall(exp.value.function).boolean;
			var.type = lexer::VarType::Boolean;
			break;
		
		case lexer::VarType::Text:
			var.text_val = interprerValueFunctionCall(exp.value.function).text;
			var.type = lexer::VarType::Text;
			break;

		case lexer::VarType::Number:
			var.number_val = interprerValueFunctionCall(exp.value.function).number;
			var.type = lexer::VarType::Number;
			break;
		
		default:
			break;
		}
		break;

	default:
		throw std::invalid_argument("Syntax error 5");
	}

	set_var(var);
}

void Interpreter::interprerBlock(parser::BlockExp exp) {
	for (parser::Statement statement : exp.statements)
		interprerStatement(statement);
}

void Interpreter::interprerWhen(parser::WhenExp exp) {
	if (interprerBoolExp(exp.exp))
		interprerBlock(exp.when_block);
	
	else if (exp.otherwise)
		interprerBlock(exp.otherwise_block);
}

void Interpreter::interprerLoop(parser::LoopExp exp) {
	size_t times = interprerArithmeticExp(exp.times);

	for (size_t counter = 0; counter < times; counter++)
		interprerBlock(exp.loop_block);
}


void Interpreter::interprerStatement(parser::Statement exp) {
	switch (exp.type)
	{
	case parser::StatementType::ASSIGNMENT_TYPE:
		interprerAssignment(exp.assignment_statement);
		break;

	case parser::StatementType::WHEN_TYPE:
		interprerWhen(exp.when_statement);
		break;

	case parser::StatementType::LOOP_TYPE:
		interprerLoop(exp.loop_statement);
		break;

	case parser::StatementType::BLOCK_TYPE:
		interprerBlock(exp.block_statement);
		break;
	
	case parser::StatementType::CALL_TYPE:
		interprerActionFunctionCall(exp.function_statement);
		break;

	default:
		throw std::invalid_argument("Syntax error 4");
	}
}

void check_params(
	const string& name,
	const size_t length,
	const vector<parser::Expression>& params
) {
	if (params.size() != length)
		throw std::invalid_argument(
			"Value error: `" + name + "` is expecting " + std::to_string(length) + " parameters, "
			+ "got " + std::to_string(params.size()) + "."
		);

}

vector<parser::ReturnType> Interpreter::interprerParams(vector<parser::Expression> params) {
	vector<parser::ReturnType> ret;
	parser::ReturnType tmp;
	
	for (auto param : params) {
		tmp = interprerExpression(param);
		ret.push_back(tmp);
	}

	return ret;
}

void Interpreter::interprerActionFunctionCall(parser::FunctionCall exp) {
	if (exp.type != lexer::FunctionType::Action)
		throw std::invalid_argument("Expecting an Action Function");
	
	vector<parser::ReturnType> parameters = interprerParams(exp.params);
	if(exp.name == "out") {
		check_params(exp.name, 1, exp.params);
		output(parameters[0]);
		return;
	}
	// else if ( ) { }
	
	throw std::invalid_argument(
		"Value error: \"" + exp.name + "\" is not a built in function"
	);
}


parser::ReturnType Interpreter::interprerValueFunctionCall(
	parser::FunctionCall exp
) {
	if (exp.type != lexer::FunctionType::Value)
		throw std::invalid_argument("Expecting a Value Function");
	
	vector<parser::ReturnType> params = interprerParams(exp.params);

	if(exp.name == "in") {
		check_params(exp.name, 1, exp.params);
		return input(params[0]);

	}

	else if(exp.name == "to-number") {
		check_params(exp.name, 1, exp.params);
		return to_number(params[0]);

	}

	else if(exp.name == "to-text") {
		check_params(exp.name, 1, exp.params);
		return to_text(params[0]);

	}

	throw std::invalid_argument(
		"Value error: \"" + exp.name + "\" is not a built in function"
	);

}

void Interpreter::interprerFile(parser::File exp) {
	for (parser::Statement statement : exp.code)
		interprerStatement(statement);
}

void Interpreter::set_var(parser::Variable& var)
{
	vars[var.name] = var;
}

bool Interpreter::is_var(string& name)
{
	return vars.find(name) != vars.end();
}

void Interpreter::interprer()
{
	interprerFile(code);
}


int main(int argc, char** argv) {
	if (argc == 2) {
		try {
			Interpreter(argv[1]).interprer();
			return 0;
		}
		
		catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
	return 1;
}