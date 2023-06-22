#include "Built-in.h"

#define PRINT(x) std::cout << x << std::endl
#define INPUT_MESSAGE "~<INPUT MESSAGE>~"

// Action Functions
void output(const parser::ReturnType& var) {
    switch (var.type) {
        case lexer::VarType::Number:
            PRINT(var.number);
            break;
        
        case lexer::VarType::Text:
            PRINT(var.text);
            break;

        case lexer::VarType::Boolean:
            if (var.boolean)
                PRINT("yes");
            
            else
                PRINT("no");
            break;

    }
    std::cout << std::flush;

}

// ------------------------------------------------- //


// Value Functions
parser::ReturnType input(const parser::ReturnType& message) {
    if (message.type != lexer::VarType::Text)
        throw std::invalid_argument("`in` expects a text param.");

    parser::ReturnType input;
    input.type = lexer::VarType::Text;

    std::cout << INPUT_MESSAGE << message.text << std::endl;
    std::cin >> input.text;
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

parser::ReturnType to_number(const parser::ReturnType& text_number) {
    if (text_number.type != lexer::VarType::Text)
        throw std::invalid_argument("`to-number` expects a text param.");

    if (!is_numeric(text_number.text))
        throw std::invalid_argument("`to-number` must receive a nemeric text");

    parser::ReturnType ret;
    size_t pos = 0;
    int value;
    bool error = false;

    try { value = std::stoi(text_number.text, &pos); }
    catch (const std::exception& e) { error = true; }

    if (pos != text_number.text.length()) error = true;

    if (error)
        throw std:: invalid_argument(
            "Conversion error: Invalid text to convert: \"" +  text_number.text + "\"."
        );

    ret.number = value;
    ret.type = lexer::VarType::Number;
    return ret;
}

parser::ReturnType to_text(const parser::ReturnType& number) {
    if (number.type != lexer::VarType::Number)
        throw std::invalid_argument("`to-text` expects a number param.");

    parser::ReturnType ret;
    string text = std::to_string(number.number);
    ret.text = text;
    ret.type = lexer::VarType::Text;
    return ret;
}