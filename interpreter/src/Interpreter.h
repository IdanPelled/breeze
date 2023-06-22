#pragma once
#include <map>
#include <iostream>
#include <string>
#include <fstream>

#include "Parser.h"
#include "Built-in.h"

#define MAX_STATEMENTS 1000

namespace interpreter
{
	class Interpreter {
	public:

		/**
		 * Constructor for the Interpreter class.
		 *
		 * @param data The code to be interpreted.
		 */
		Interpreter(const string& code);


		/**
		 * Interprets the code by executing the statements in the file.
		 *
		 * This function interprets the code by executing each statement in the file.
		 * It starts by calling the `interprerFile` function to parse the code into statements,
		 * and then it iterates over each statement and calls the `interprerStatement` function
		 * to execute it.
		 */
		void interprer();
		

		/**
		 * Sets the value of a variable in the symbol table.
		 *
		 * This function sets the value of a variable in the symbol table. If the variable
		 * already exists, its value is updated. If the variable does not exist, a new entry
		 * is created in the symbol table.
		 *
		 * @param var The variable to set.
		 */
		void set_var(parser::Variable& var);


		/**
		 * Checks if a variable exists in the symbol table.
		 *
		 * This function checks if a variable with the given name exists in the symbol table.
		 *
		 * @param name The name of the variable to check.
		 * @return true if the variable exists, false otherwise.
		 */
		bool is_var(string& name);


		/**
		 * Retrieves a variable from the symbol table.
		 *
		 * This function retrieves the variable with the given name from the symbol table.
		 * If the variable does not exist in the symbol table, an std::invalid_argument exception
		 * is thrown with an appropriate error message.
		 *
		 * @param name The name of the variable to retrieve.
		 * @return A reference to the Variable object in the symbol table.
		 * @throws std::invalid_argument if the variable is undefined.
		 */
		parser::Variable& get_var(string name);


		/**
		 * Interprets a number expression.
		 *
		 * This function interprets the given NumberExp object representing a number expression.
		 * It handles two types of number expressions: Func and Token. For a Func number expression,
		 * it calls the interpretValueFunctionCall() function to evaluate the value function and checks
		 * if the result is a number. If it's not a number, an std::invalid_argument exception is thrown.
		 * For a Token number expression, it checks the type of the token and retrieves the corresponding
		 * variable value from the symbol table if it's a variable token, or converts the number literal
		 * to an integer if it's a number literal token. The final result of the number expression is returned.
		 *
		 * @param exp The NumberExp object representing the number expression.
		 * @return The evaluated integer result of the number expression.
		 * @throws std::invalid_argument if there is a syntax error or an invalid token.
		 */
		int interprerNumberExp(parser::NumberExp exp);


		/**
		 * Interprets a multiply expression.
		 *
		 * This function interprets the given MulExp object representing a multiply expression.
		 * It calls the interpretNumberExp() function to evaluate the number expression and
		 * returns the result.
		 *
		 * @param exp The MulExp object representing the multiply expression.
		 * @return The evaluated integer result of the multiply expression.
		 */
		int interprerMulExp(parser::MulExp exp);



		/**
		 * Interprets a plus expression.
		 *
		 * This function interprets the given PlusExp object representing a plus expression.
		 * It calls the interpretNumExp() function to evaluate the num expression and returns
		 * the result.
		 *
		 * @param exp The PlusExp object representing the plus expression.
		 * @return The evaluated integer result of the plus expression.
		 */
		int interprerPlusExp(parser::PlusExp exp);

		/**
		 * Interprets a num expression.
		 *
		 * This function interprets the given NumExp object representing a num expression.
		 * It evaluates the number expression using the interpretNumberExp() function and
		 * then iterates through each MulExp in the expression, evaluating them using the
		 * interpretMulExp() function and multiplying the result. The final result of the
		 * num expression is returned.
		 *
		 * @param exp The NumExp object representing the num expression.
		 * @return The evaluated integer result of the num expression.
		 */
		int interprerNumExp(parser::NumExp exp);


		/**
		 * Interprets an arithmetic expression.
		 *
		 * This function interprets the given ArithmeticExp object representing an arithmetic expression.
		 * It evaluates the first numerical expression using the interpretNumExp() function and assigns
		 * the result to the initial value. Then, it iterates through each PlusExp in the expression and
		 * evaluates the numerical expression using the interpretPlusExp() function. If the PlusExp is
		 * a plus operation, the value is added to the result. If the PlusExp is a minus operation, the
		 * value is subtracted from the result. The final result of the arithmetic expression is returned.
		 *
		 * @param exp The ArithmeticExp object representing the arithmetic expression.
		 * @return The evaluated integer result of the arithmetic expression.
		 */
		int interprerArithmeticExp(parser::ArithmeticExp exp);


		/**
		 * Interprets an expression.
		 *
		 * This function interprets the given Expression object and evaluates it to a ReturnType object.
		 * The function handles different types of expressions, including identifiers, boolean expressions,
		 * string literals, integer arithmetic expressions, and function calls. For an identifier expression,
		 * it retrieves the corresponding variable value from the symbol table and assigns it to the ReturnType.
		 * For a boolean expression, it calls the interpretBoolExp() function to evaluate the boolean value.
		 * For a string literal, it assigns the string value to the ReturnType. For an integer arithmetic
		 * expression, it calls the interpretArithmeticExp() function to evaluate the arithmetic expression.
		 * For a function call, it calls the interpretValueFunctionCall() function to evaluate the function call
		 * and returns the result.
		 *
		 * @param exp The Expression object to interpret.
		 * @return The evaluated ReturnType object representing the expression.
		 */
		parser::ReturnType interprerExpression(parser::Expression exp);


		/**
		 * Interprets an operand.
		 *
		 * This function interprets the given Operand object representing an operand in an expression.
		 * It handles two types of operands: Expression and Literal. For an Expression operand, it calls
		 * the interpretExpression() function to evaluate the expression and returns the result. For a
		 * Literal operand, it evaluates the literal value and returns a ReturnType object containing
		 * the corresponding boolean value.
		 *
		 * @param exp The Operand object representing the operand.
		 * @return The evaluated ReturnType object representing the operand.
		 */
		parser::ReturnType interprerOperand(parser::Operand exp);


		/**
		 * Interprets a boolean expression.
		 *
		 * This function interprets the given BoolExp object representing a boolean expression.
		 * It evaluates the left operand using the interpretOperand() function and then iterates
		 * through each operand in the expression, evaluating the right operand and applying
		 * the corresponding boolean operator. The function supports equality, greater than, and
		 * smaller than comparisons. The result of the final evaluation is returned as a boolean value.
		 *
		 * @param exp The BoolExp object representing the boolean expression.
		 * @return The evaluated boolean result of the expression.
		 * @throws std::invalid_argument if there is a syntax error or an unsupported operator.
		 */
		bool interprerBoolExp(parser::BoolExp exp);


		/**
		 * Evaluates a boolean string.
		 *
		 * This function evaluates the given string as a boolean value. If the string is "yes",
		 * it returns true. If the string is "no", it returns false. If the string does not match
		 * any of these valid boolean values, an std::invalid_argument exception is thrown.
		 *
		 * @param s The string to evaluate as a boolean.
		 * @return The boolean value corresponding to the string.
		 * @throws std::invalid_argument if the string is not a valid boolean argument.
		 */
		bool evaluate_bool(string s);

		/**
		 * Interprets an assignment expression.
		 *
		 * This function interprets the given AssignmentExp object representing an assignment expression.
		 * It evaluates the value expression based on the assignment type and assigns the result to
		 * the corresponding variable. The function handles different types of assignments, including
		 * integer, string, boolean, identifier, and function assignments.
		 *
		 * @param exp The AssignmentExp object representing the assignment expression.
		 * @throws std::invalid_argument if the assignment type or value is not recognized.
		 */
		void interprerAssignment(parser::AssignmentExp exp);


		/**
		 * Interprets a block expression.
		 *
		 * This function interprets the given BlockExp object representing a block of statements.
		 * It iterates through each statement in the block and calls the interpretStatement()
		 * function to interpret and execute each individual statement.
		 *
		 * @param exp The BlockExp object representing the block of statements.
		 */
		void interprerBlock(parser::BlockExp exp);


		/**
		 * Interprets a when expression.
		 *
		 * This function interprets the given WhenExp object representing a when expression.
		 * It evaluates the boolean expression in the "exp" field using the interpretBoolExp() function.
		 * If the boolean expression evaluates to true, it interprets and executes the "when_block".
		 * If the boolean expression evaluates to false and an "otherwise_block" is provided, it
		 * interprets and executes the "otherwise_block". If the boolean expression evaluates to false
		 * and no "otherwise_block" is provided, the function does nothing.
		 *
		 * @param exp The WhenExp object representing the when expression.
		 */
		void interprerWhen(parser::WhenExp exp);


		/**
		 * Interprets a while expression.
		 *
		 * This function interprets the given WhileExp object representing a while expression.
		 * It evaluates the boolean expression in the "exp" field using the interpretBoolExp() function.
		 * While the boolean expression evaluates to true, it interprets and executes the "while_block".
		 *
		 * @param exp The WhileExp object representing the while expression.
		 */
		void interprerWhile(parser::WhileExp exp);


		/**
		 * Interprets a loop expression.
		 *
		 * This function interprets the given LoopExp object representing a loop expression.
		 * It evaluates the number of times the loop should run by interpreting the arithmetic expression
		 * in the "times" field. Then, it iterates over the loop block, executing it the specified number
		 * of times.
		 *
		 * @param exp The LoopExp object representing the loop expression.
		 */
		void interprerLoop(parser::LoopExp exp);


		/**
		 * Interprets an action function call.
		 *
		 * This function interprets the given FunctionCall object representing an action function call.
		 * It verifies that the function call type is "Action" and then performs the appropriate action
		 * based on the function name and parameters. The function performs different actions
		 * based on the function name and does not return a value.
		 *
		 * @param exp The FunctionCall object representing the action function call.
		 * @throws std::invalid_argument if the function type is not "Action" or if the function name is not recognized.
		 */
		void interprerActionFunctionCall(parser::FunctionCall exp);


		/**
		 * Interprets a value function call.
		 *
		 * This function interprets the given FunctionCall object representing a value function call.
		 * It verifies that the function call type is "Value" and then evaluates the specific value
		 * function based on its name and parameters. The function performs different actions
		 * based on the function name and returns the appropriate ReturnType.
		 *
		 * @param exp The FunctionCall object representing the value function call.
		 * @return The ReturnType based on the evaluated value function.
		 * @throws std::invalid_argument if the function type is not "Value" or if the function name is not recognized.
		 */
		parser::ReturnType interprerValueFunctionCall(parser::FunctionCall exp);


		/**
		 * Interprets the parameters of a function call.
		 *
		 * This function interprets the given vector of Expression objects, representing
		 * the parameters of a function call. It iterates through each parameter, interprets
		 * the expression using the interpreter, and collects the return types into a vector.
		 *
		 * @param params The vector of Expression objects representing the function call parameters.
		 * @return A vector of ReturnType objects representing the interpreted parameters.
		 */
		vector<parser::ReturnType> interprerParams(vector<parser::Expression> params);


		/**
		 * Interprets a statement.
		 *
		 * This function interprets the given Statement object based on its type.
		 * It performs different actions depending on the type of statement,
		 * such as assignment, when, loop, block, or function call.
		 *
		 * @param exp The Statement object to be interpreted.
		 * @throws std::invalid_argument if the statement type is not recognized.
		 */
		void interprerStatement(parser::Statement exp);


		/**
		 * Interprets a file.
		 *
		 * This function interprets the given File object by iterating through the
		 * list of statements in the file and calling the interpreter to process each
		 * individual statement.
		 *
		 * @param exp The File object to be interpreted.
		 */
		void interprerFile(parser::File exp);


	private:

		map<string, parser::Variable> vars;
		parser::File code;
	};
}