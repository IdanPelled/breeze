#pragma once
#include "Parser.h"


// Action Functions
void output(const parser::ReturnType& var);


// Value Functions
parser::ReturnType input(const parser::ReturnType& message);
parser::ReturnType to_number(const parser::ReturnType& text_number);
parser::ReturnType to_text(const parser::ReturnType& number);