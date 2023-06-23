#include "Lexer.h"

using namespace lexer;

#define KEYWORD_REGEX R"(set|to|when|do|otherwise|loop|times|while)"
#define IDENTIFIER_REGEX R"((out|in|to-number|to-text)|[a-zA-Z][a-zA-Z0-9_-]*)"
#define LITERAL_REGEX R"(no|yes|-?\d+|\"(?:\\.|[^\"])*\")"
#define OPERATOR_REGEX R"([><=+\-*\/])"
#define PUNCTUATION_REGEX R"([\{\}\(\)])"
#define COMMENT_REGEX R"(\/\/.*)"
#define FULL_REGEX "(" COMMENT_REGEX "|" IDENTIFIER_REGEX "|" KEYWORD_REGEX "|" LITERAL_REGEX "|" OPERATOR_REGEX "|" PUNCTUATION_REGEX "|[^a-zA-Z0-9\\s])"

map<string, TokenType> keywords {
    {"when", TokenType::WHEN},
    {"do", TokenType::DO},
    {"otherwise", TokenType::OTHERWISE},
    {"loop", TokenType::LOOP},
    {"times", TokenType::TIMES},
    {"set", TokenType::SET},
    {"to", TokenType::TO},
    {"while", TokenType::WHILE},
};

map<string, TokenType> operators {
    {"+", TokenType::PLUS},
    {"-", TokenType::MINUS},
    {"*", TokenType::MULTIPLY},
    {"/", TokenType::DIVIDE},
    {"=", TokenType::EQUAL},
    {">", TokenType::GREATER},
    {"<", TokenType::SMALLER},
};

map<string, TokenType> punctuations {
    {"{", TokenType::OPEN},
    {"}", TokenType::CLOSE},
    {"(", TokenType::CALL_OPEN},
    {")", TokenType::CALL_CLOSE},
    {",", TokenType::COMMA},
};

map<FunctionType, map<string, map<string, VarType>>> functions = {
	{
		FunctionType::Action,
		{
			{"out", {
				{"param", VarType::Text}
			}}
		}
	},
	{
		FunctionType::Value,
		{
			{"in", {
				{"param", VarType::Text},
				{"return", VarType::Text}
			}},
			{"to-number", {
				{"param", VarType::Text},
				{"return", VarType::Number}
			}},
			{"to-text", {
				{"param", VarType::Number},
				{"return", VarType::Text}
			}}
		}
	}
};

void unexpected_token(const lexer::Token& token) {
    throw std::invalid_argument("Unexpected token: \"" + token.value + "\"");
}

Lexer::Lexer() {}

const vector<Token> Lexer::lex(const string& code) {
	vector<Token> tokens;
	tokenize(tokens, code);
	classify(tokens);

	return tokens;
}

void Lexer::tokenize(vector<Token>& tokens, const string& input) {
    
    std::regex keywordRegex(KEYWORD_REGEX);
    std::regex identifierRegex(IDENTIFIER_REGEX);
    std::regex literalRegex(LITERAL_REGEX);
    std::regex operatorRegex(OPERATOR_REGEX);
    std::regex punctuationRegex(PUNCTUATION_REGEX);
    std::regex commentRegex(COMMENT_REGEX);

    std::regex tokenRegex(FULL_REGEX);

    auto words_begin = std::sregex_iterator(input.begin(), input.end(), tokenRegex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator it = words_begin; it != words_end; ++it) {
        std::smatch match = *it;
        std::string token = match.str();

        Token t;
        t.value = token;

        if (std::regex_match(token, commentRegex)) {
            continue;
        }

        if (std::regex_match(token, keywordRegex)) {
            t.category = TokenCategory::KEYWORD;

        } else if (std::regex_match(token, literalRegex)) {
            t.category = TokenCategory::LITERAL;

        } else if (std::regex_match(token, identifierRegex)) {
            t.category = TokenCategory::IDENTIFIER;

        } else if (std::regex_match(token, operatorRegex)) {
            t.category = TokenCategory::OPERATOR;

        } else if (std::regex_match(token, punctuationRegex)) {
            t.category = TokenCategory::PUNCTUATION;
        } else {
            throw std::runtime_error("Invalid token found: " + token);
        }

        tokens.push_back(t);
    }
}

template<typename T>
inline bool Lexer::is_in_map(const string& key, const map<string, T>& dict) {
    return dict.find(key) != dict.end();
}

TokenType Lexer::get_keyword_type(const Token& token) {
    if (!is_in_map<TokenType>(token.value, keywords))
        unexpected_token(token);
    
    return keywords.at(token.value);
}

TokenType Lexer::get_operator_type(const Token& token) {
    if (!is_in_map<TokenType>(token.value, operators))
        unexpected_token(token);
    
    return operators.at(token.value);
}

TokenType Lexer::get_punctuation_type(const Token& token) {
    if (!is_in_map<TokenType>(token.value, punctuations))
        unexpected_token(token);
    
    return punctuations.at(token.value);
}

bool Lexer::is_function(const Token& token) {
    for (auto pair : functions) {
        if (is_in_map<map<string, VarType>>(token.value, pair.second))
            return true;
    }

    return false;
}

FunctionType Lexer::get_function_type(const Token& token) {
    for (auto pair : functions) {
        if (is_in_map<map<string, VarType>>(token.value, pair.second))
            return pair.first;
    }

    unexpected_token(token);
}

TokenType Lexer::get_identifier_type(const Token& token) {
    if (is_function(token)) {
        FunctionType type = get_function_type(token);
        
        if (type == FunctionType::Action)
            return TokenType::ACTION_FUNCTION;
        
        if (type == FunctionType::Value)
            return TokenType::VALUE_FUNCTION;

        unexpected_token(token);
    }
    
    return TokenType::VARIABLE;
}

inline bool Lexer::is_bool_literal(const string& str) {
    return (str == "yes" || str == "no");
}

inline bool Lexer::is_text_literal(const string& str) {
    return str[0] == '"';
}

TokenType Lexer::get_literal_type(Token& token) {
    if (Lexer::is_bool_literal(token.value))
        return TokenType::BOOLEAN_LITERAL;
    
    else if (is_text_literal(token.value)){
        token.value = token.value.substr(1, token.value.length() - 2);
        return TokenType::TEXT_LITERAL;
    }


    return TokenType::NUMBER_LITERAL;
}

void Lexer::classify(vector<Token>& tokens) {
    for (Token& token : tokens) {
        
        switch (token.category) {
            case TokenCategory::KEYWORD:
                token.type = get_keyword_type(token);
                break;

            case TokenCategory::IDENTIFIER:
                token.type = get_identifier_type(token);
                break;

            case TokenCategory::LITERAL:
                token.type = get_literal_type(token);
                break;

            case TokenCategory::OPERATOR:
                token.type = get_operator_type(token);
                break;

            case TokenCategory::PUNCTUATION:
                token.type = get_punctuation_type(token);
                break;

            default:
                unexpected_token(token);
                break;
        }
    }
}
