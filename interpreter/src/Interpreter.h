#pragma once
#include <map>
#include <iostream>
#include <string>
#include <fstream>

#include "Parser.h"
#include "Built-in.h"


namespace interpreter
{
	class Interpreter {
	public:
		Interpreter(const string& code);
		void interprer();
		
		void set_var(parser::Variable& var);
		bool is_var(string& name);
		parser::Variable& get_var(string name);

		int interprerNumberExp(parser::NumberExp exp);
		int interprerMulExp(parser::MulExp exp);
		int interprerPlusExp(parser::PlusExp exp);
		int interprerNumExp(parser::NumExp exp);
		int interprerArithmeticExp(parser::ArithmeticExp exp);
		
		parser::ReturnType interprerExpression(parser::Expression exp);
		parser::ReturnType interprerOperand(parser::Operand exp);
		bool interprerBoolExp(parser::BoolExp exp);
		bool evaluate_bool(string s);

		void interprerAssignment(parser::AssignmentExp exp);
		void interprerBlock(parser::BlockExp exp);
		void interprerWhen(parser::WhenExp exp);
		void interprerLoop(parser::LoopExp exp);

		void interprerActionFunctionCall(parser::FunctionCall exp);
		parser::ReturnType interprerValueFunctionCall(parser::FunctionCall exp);
		vector<parser::ReturnType> interprerParams(vector<parser::Expression> params);

		void interprerStatement(parser::Statement exp);
		void interprerFile(parser::File exp);

	private:
		map<string, parser::Variable> vars;
		parser::File code;
	};
}