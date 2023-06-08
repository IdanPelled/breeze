#pragma once
#include <map>
#include <iostream>
#include <string>

#include "Parser.h"

using std::map;
using std::string;

namespace interpreter
{
	typedef enum class VarType {
		Integer,
		String,
		Boolean,
	} var_t;

	struct Variable {
		string name;

		int int_val;
		string identifier_val;
		bool bool_val;
		string string_val;
	
		var_t type;
	};

	struct ReturnType {
		string string;
		int integer;
		bool boolean;

		var_t type;
	};

	class Interpreter {
	public:
		Interpreter(const string& code);
		void interprer();
		
		void set_vat(Variable& var);
		bool is_var(string& name);
		Variable get_var(string name);


		int interprerMulExp(parser::MulExp exp);
		int interprerPlusExp(parser::PlusExp exp);
		int interprerNumExp(parser::NumExp exp);
		int interprerArithmeticExp(parser::ArithmeticExp exp);
		
		ReturnType interprerExpression(parser::Operand exp);
		bool interprerBoolExp(parser::BoolExp exp);
		bool evaluate_bool(string s);

		void interprerAssignment(parser::AssignmentExp exp);
		void interprerBlock(parser::BlockExp exp);
		void interprerIf(parser::IfExp exp);
		void interprerStatement(parser::Statement exp);
		void interprerFile(parser::File exp);

	private:
		map<string, Variable> vars;
		parser::File code;
	};
}