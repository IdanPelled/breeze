#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <regex>
#include <vector>
#include <stdexcept>
#include <map>

using std::string;
using std::map;
using std::vector;


namespace lexer
{
    enum class TokenCategory {
		KEYWORD,
		IDENTIFIER,
		LITERAL,
		OPERATOR,
		PUNCTUATION
	};

	enum class FunctionType {
		Action,
		Value,
	};

	enum class VarType {
		Number,
		Text,
		Boolean,
	};

	enum class TokenType {
		// KeyWords:
		WHEN,
		DO,
		OTHERWISE,
		LOOP,
		TIMES,
		SET,
		TO,

		// Punctuation
		OPEN,
		CLOSE,
		CALL_OPEN,
		CALL_CLOSE,
		COMMA,

		// Identifiers:
		VARIABLE,
		ACTION_FUNCTION,
		VALUE_FUNCTION,

		// Literals:
		NUMBER_LITERAL,
		TEXT_LITERAL,
		BOOLEAN_LITERAL,

		// Operators
		PLUS,
		MINUS,
		MULTIPLY,
		DIVIDE,
		EQUAL,
		GREATER,
		SMALLER,

		// End of file token
		END_OF_TOKENS,
	};

	class Token {
	public:
		TokenCategory category;
		TokenType type;
		std::string value;
	};

    class Lexer {
	public:
		Lexer();
		const vector<Token> lex(const string& code);

	private:
        template<typename T>
        inline bool is_in_map(const string& key, const map<string, T>& dict);

        TokenType get_keyword_type(const Token& token);
        TokenType get_operator_type(const Token& token);
        TokenType get_punctuation_type(const Token& token);
        TokenType get_identifier_type(const Token& token);
        TokenType get_literal_type(Token& token);
        FunctionType get_function_type(const Token& token);
		inline bool is_bool_literal(const string& str);
		inline bool is_text_literal(const string& str);
        bool is_function(const Token& token);

        void classify(vector<Token>& tokens);
		void tokenize(vector<Token>& tokens, const string& input);
	};
}

void unexpected_token(const lexer::Token& token);
extern map<lexer::FunctionType, map<string, map<string, lexer::VarType>>> functions;