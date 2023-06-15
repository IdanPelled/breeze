#pragma once
#include <map>
#include <iostream>
#include <string>
#include <fstream>

#include "Parser.h"
#include "Lexer.h"
#include "Built-in.h"


using std::map;
using std::string;

namespace interpreter
{
	class Interpreter {
	public:
		Interpreter(const string& code);
		void interprer();
		
		void set_vat(Variable& var);
		bool is_var(string& name);
		Variable& get_var(string name);

		int interprerNumberExp(parser::NumberExp exp);
		int interprerMulExp(parser::MulExp exp);
		int interprerPlusExp(parser::PlusExp exp);
		int interprerNumExp(parser::NumExp exp);
		int interprerArithmeticExp(parser::ArithmeticExp exp);
		
		ReturnType interprerExpression(parser::Expression exp);
		ReturnType interprerOperand(parser::Operand exp);
		bool interprerBoolExp(parser::BoolExp exp);
		bool evaluate_bool(string s);

		void interprerAssignment(parser::AssignmentExp exp);
		void interprerBlock(parser::BlockExp exp);
		void interprerWhen(parser::WhenExp exp);
		void interprerLoop(parser::LoopExp exp);

		void interprerActionFunctionCall(parser::FunctionCall exp);
		ReturnType interprerValueFunctionCall(parser::FunctionCall exp);
		vector<ReturnType> interprerParams(vector<parser::Expression> params);

		void interprerStatement(parser::Statement exp);
		void interprerFile(parser::File exp);

	private:
		map<string, Variable> vars;
		parser::File code;
	};
}