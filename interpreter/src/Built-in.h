#pragma once
#include "Parser.h"


// Action Functions

/**
 * Outputs the value of a variable to the console.
 * Supports variables of type `Number`, `Text`, and `Boolean`.
 * 
 * @param var The variable to be outputted.
 * @throws std::invalid_argument if the variable is of an unsupported type.
 */
void output(const parser::ReturnType& var);


// -----------------------

// Value Functions


/**
 * Reads user input from the console.
 * 
 * @param message The message displayed as an input prompt.
 * @return The user's input as a `parser::ReturnType` with type `Text`.
 * @throws std::invalid_argument if the input parameter is not of type `Text`.
 */
parser::ReturnType input(const parser::ReturnType& message);


/**
 * Checks if a string represents a numeric value.
 * 
 * @param str The string to check.
 * @return `true` if the string represents a numeric value, `false` otherwise.
 */
parser::ReturnType to_number(const parser::ReturnType& text_number);


/**
 * Converts a text representation of a number to a numeric value.
 * 
 * @param text_number The text representation of the number.
 * @return The converted numeric value as a `parser::ReturnType` with type `Number`.
 * @throws std::invalid_argument if the input text is not a valid numeric representation.
 * @throws std::invalid_argument if the input parameter is not of type `Text`.
 */
parser::ReturnType to_text(const parser::ReturnType& number);