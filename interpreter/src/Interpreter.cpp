#include "Interpreter.h"

using namespace interpreter;

const string read_file(const string& path) {
	std::ifstream file;
	std::stringstream buffer;

	file.open(path);
	buffer << file.rdbuf();
	return buffer.str();
}

Interpreter::Interpreter(const string& data)
	: code { parser::Parser(data).parseFile() }
{
}

Variable& Interpreter::get_var(string name) {
	try {
		return vars.at(name);
	}
	
	catch (const std::out_of_range& e) {
        throw std::invalid_argument("Key error: \"" + name + "\"");
    }
	
}

int Interpreter::interprerMulExp(parser::MulExp exp) {
	return stoi(exp.value.get_data("value"));
}

int Interpreter::interprerPlusExp(parser::PlusExp exp) {
	return interprerNumExp(exp.num);

}



int Interpreter::interprerNumExp(parser::NumExp exp) {
	int ret;

	if (exp.value.get_type() == token::TokenType::IDENTI) {
		Variable var = get_var(exp.value.get_data("name"));
		
		if (var.type != VarType::Integer)
			throw std::invalid_argument("Variable must be a number");
		
		ret = var.int_val;
	}

	else
		ret = stoi(exp.value.get_data("value"));
	

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

ReturnType Interpreter::interprerExpression(parser::Expression exp) {
	ReturnType ret;
	Variable var;

	switch (exp.type)
	{
	case parser::Type::IDENTIFIER:
		var = get_var(exp.identifier.get_data("name"));

		switch (var.type)
		{
		case VarType::Integer:
			ret.integer = var.int_val;
			break;

		case VarType::Boolean:
			ret.boolean = var.bool_val;
			break;

		case VarType::String:
			ret.String = var.string_val;
			break;
		}
		ret.type = var.type;
		break;
	
	case parser::Type::BOOLEAN:
		ret.type = VarType::Boolean;

		if(exp.boolean.operands.size() == 0)
			ret.boolean = interprerOperand(exp.boolean.left).boolean;

		ret.boolean = interprerBoolExp(exp.boolean);
		break;

	case parser::Type::STRING:
		ret.String = exp.string.get_data("value");
		ret.type = VarType::String;
		break;

	case parser::Type::INTEGER:
		ret.integer = interprerArithmeticExp(exp.math_exp);
		ret.type = VarType::Integer;
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


bool evaluate_equal(ReturnType left, ReturnType right) {
	if (left.type != right.type)
		throw std::invalid_argument("Value error: Cant compare diffrent types");

	switch (left.type)
	{
	case VarType::Boolean:
		return left.boolean == right.boolean;

	case VarType::String:
		return left.String.compare(right.String) == 0;

	case VarType::Integer:
		return left.integer == right.integer;
	
	default:
		throw std::invalid_argument("Invalid type");
	}
}

bool evaluate_grater(ReturnType left, ReturnType right) {
	if (left.type != right.type)
		throw std::invalid_argument("Value error: Cant compare diffrent types");

	switch (left.type)
	{
	case VarType::Boolean:
		throw std::invalid_argument("Cant use operator < on type boolean");

	case VarType::String:
		throw std::invalid_argument("Cant use operator < on type string");

	case VarType::Integer:
		return left.integer > right.integer;

	default:
		throw std::invalid_argument("Invalid type");
	}
}

bool evaluate_smaller(ReturnType left, ReturnType right) {
	if (left.type != right.type)
		throw std::invalid_argument("Value error: Cant compare diffrent types");

	switch (left.type)
	{
	case VarType::Boolean:
		throw std::invalid_argument("Cant use operator > on type boolean");

	case VarType::String:
		throw std::invalid_argument("Cant use operator > on type string");

	case VarType::Integer:
		return left.integer < right.integer;

	default:
		throw std::invalid_argument("Invalid type");
	}
}

ReturnType Interpreter::interprerOperand(parser::Operand exp) {
	ReturnType ret;
	
	switch (exp.type)
	{
	case parser::OperandType::Expression:
		return interprerExpression(*exp.exp_value);
	
	case parser::OperandType::Literal:
		ret.type = VarType::Boolean;
		ret.boolean = evaluate_bool(exp.literal_value.get_data("value"));
		break;
	}

	return ret;
}


bool Interpreter::interprerBoolExp(parser::BoolExp exp) {
	ReturnType left = interprerOperand(exp.left);
	ReturnType right, tmp;
	bool ret = false;

	for(auto operand : exp.operands) {
		right = interprerOperand(operand);
		
		switch (operand.op.get_type())
		{
		case token::TokenType::EQUAL:
			ret = evaluate_equal(left, right);
			break;

		case token::TokenType::GREATER:
			ret = evaluate_grater(left, right);
			break;

		case token::TokenType::SMALLER:
			ret = evaluate_smaller(left, right);
			break;

		default:
			throw std::invalid_argument("Syntax error");
		}

		tmp.type = VarType::Boolean;
		tmp.boolean = ret;
		left = tmp;

	}
	ret = left.boolean;
	return ret;
}

map<string, token::TokenType> get_function(const string& name) {
	for (auto type: functions) {
		
		for (auto pair: type.second) {			
			if (pair.first == name)
				return pair.second;
		}
	}

	throw std::invalid_argument("Function does not exist");
}

void Interpreter::interprerAssignment(parser::AssignmentExp exp) {
	Variable var;

	var.name = exp.identifier.get_data("name");
	switch (exp.type)
	{
	case parser::Type::INTEGER:
		var.int_val = interprerArithmeticExp(exp.value.math_exp);
		var.type = VarType::Integer;
		break;

	case parser::Type::STRING:
		var.string_val = exp.value.string.get_data("value");
		var.type = VarType::String;
		break;

	case parser::Type::BOOLEAN:
		var.bool_val = interprerBoolExp(exp.value.boolean);
		var.type = VarType::Boolean;
		break;

	case parser::Type::IDENTIFIER:
		switch (get_var(exp.value.identifier.get_data("name")).type)
		{
		case VarType::Integer:
			std::cout << exp.value.identifier.get_data("name");
			var.int_val = interprerArithmeticExp(exp.value.math_exp);
			var.type = VarType::Integer;
			break;

		case VarType::String:
			var.string_val = exp.value.string.get_data("value");
			var.type = VarType::String;
			break;

		case VarType::Boolean:
			var.bool_val = interprerBoolExp(exp.value.boolean);
			var.type = VarType::Boolean;
			break;

		default:
			break;
		}
		break;

	case parser::Type::FUNCTION:
		switch (get_function(exp.value.function.name)["return"])
		{
		case token::TokenType::BOOL:
			var.int_val = interprerValueFunctionCall(exp.value.function).boolean;
			var.type = VarType::Boolean;
			break;
		
		case token::TokenType::TEXT:
			var.string_val = interprerValueFunctionCall(exp.value.function).String;
			var.type = VarType::String;
			break;

		case token::TokenType::NUMBER:
			var.int_val = interprerValueFunctionCall(exp.value.function).integer;
			var.type = VarType::Integer;
			break;
		
		default:
			break;
		}
		break;

	default:
		throw std::invalid_argument("Syntax error 5");
	}

	set_vat(var);
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

void Interpreter::interprerStatement(parser::Statement exp) {
	switch (exp.type)
	{
	case parser::StatementType::ASSIGNMENT_TYPE:
		interprerAssignment(exp.assignment_statement);
		break;

	case parser::StatementType::WHEN_TYPE:
		interprerWhen(exp.when_statement);
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

vector<ReturnType> Interpreter::interprerParams(vector<parser::Expression> params) {
	vector<ReturnType> ret;
	ReturnType tmp;
	
	for (auto param : params) {
		tmp = interprerExpression(param);
		ret.push_back(tmp);
	}
	return ret;
}

void Interpreter::interprerActionFunctionCall(parser::FunctionCall exp) {
	if (exp.type != parser::FuncType::Action)
		throw std::invalid_argument("Expecting an Action Function");
	
	vector<ReturnType> parameters = interprerParams(exp.params);

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


ReturnType Interpreter::interprerValueFunctionCall(
	parser::FunctionCall exp
) {
	if (exp.type != parser::FuncType::Value)
		throw std::invalid_argument("Expecting a Value Function");
	
	vector<ReturnType> params = interprerParams(exp.params);

	if(exp.name == "in") {
		check_params(exp.name, 1, exp.params);
		return input(params[0]);

	}

	// else if ( ) { }

	throw std::invalid_argument(
		"Value error: \"" + exp.name + "\" is not a built in function"
	);

}

void Interpreter::interprerFile(parser::File exp) {
	for (parser::Statement statement : exp.code)
		interprerStatement(statement);
}

void Interpreter::set_vat(Variable& var)
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