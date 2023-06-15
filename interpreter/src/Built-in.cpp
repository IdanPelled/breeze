#include "Built-in.h"

#define PRINT(x) std::cout << x << std::endl
#define INPUT_MESSAGE "~<INPUT MESSAGE>~"

// Action Functions
void output(const ReturnType& var) {
    switch (var.type) {
        case VarType::Integer:
            PRINT(var.integer);
            break;
        
        case VarType::String:
            PRINT(var.String);
            break;

        case VarType::Boolean:
            if (var.boolean)
                PRINT("yes");
            
            else
                PRINT("no");
            break;
        
        default:
            throw std::invalid_argument("Syntax error");

    }
    std::cout << std::flush;

}

// ------------------------------------------------- //


// Value Functions
ReturnType input(const ReturnType& message) {
    if (message.type != VarType::String)
        throw std::invalid_argument("`in` expects a text param.");

    ReturnType input;
    input.type = VarType::String;

    std::cout << INPUT_MESSAGE << message.String << std::endl;
    std::cin >> input.String;
    return input;
}

bool is_numeric(const string& str) {
    for (char c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

ReturnType to_number(const ReturnType& text_number) {
    if (text_number.type != VarType::String)
        throw std::invalid_argument("`to-number` expects a text param.");

    if (!is_numeric(text_number.String))
        throw std::invalid_argument("`to-number` must receive a nemeric text");

    ReturnType ret;
    size_t pos = 0;
    int value;
    bool error = false;

    try { value = std::stoi(text_number.String, &pos); }
    catch (const std::exception& e) { error = true; }

    if (pos != text_number.String.length()) error = true;

    if (error)
        throw std:: invalid_argument(
            "Conversion error: Invalid text to convert: \"" +  text_number.String + "\"."
        );

    ret.integer = value;
    ret.type = VarType::Integer;
    return ret;
}

ReturnType to_text(const ReturnType& number) {
    if (number.type != VarType::Integer)
        throw std::invalid_argument("`to-text` expects a number param.");

    ReturnType ret;
    string text = std::to_string(number.integer);
    ret.String = text;
    ret.type = VarType::String;
    return ret;
}