#pragma once
#include "Token.h"
#include "Parser.h"

typedef enum class VarType {
    Integer,
    String,
    Boolean,
} var_t;

struct Variable {
    string name;

    int int_val;
    bool bool_val;
    string string_val;

    var_t type;
};

struct ReturnType {
    string String;
    int integer;
    bool boolean;

    var_t type;
};

// Action Functions
void output(const ReturnType& var);


// Value Functions
ReturnType input(const ReturnType& message);
ReturnType to_number(const ReturnType& text_number);
ReturnType to_text(const ReturnType& number);