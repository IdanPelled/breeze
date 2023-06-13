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
    }
}

// ------------------------------------------------- //


// Value Functions
ReturnType input(const ReturnType& message) {
    if (message.type != VarType::String)
        throw std::invalid_argument("`in` expects a text param.");

    ReturnType input;
    std::cout << INPUT_MESSAGE << message.String << std::endl;
    std::cin >> input.String;
    return input;
}
