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
		WHILE,

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
		/**
		 * Default constructor for the Lexer class.
		 */
		Lexer();

		/**
		 * Lexically analyzes the given code and returns a vector of tokens.
		 *
		 * @param code [in] The code to lexically analyze.
		 * @return A vector of Token objects representing the lexemes found in the code.
		 */
		const vector<Token> lex(const string& code);

	private:

		/**
		 * Checks if the given key exists in the provided map.
		 *
		 * @tparam T The value type of the map.
		 * @param key [in] The key to check.
		 * @param dict [in] The map to search in.
		 * @return `true` if the key is found in the map, `false` otherwise.
		 */
        template<typename T>
        inline bool is_in_map(const string& key, const map<string, T>& dict);

		/**
		 * Retrieves the keyword type associated with the given token.
		 *
		 * @param token [in] The token for which to retrieve the keyword type.
		 * @return The keyword type associated with the token.
		 * @throws std::runtime_error if the keyword type for the token is not found.
		 */
        TokenType get_keyword_type(const Token& token);

		/**
		 * Retrieves the operator type associated with the given token.
		 *
		 * @param token [in] The token for which to retrieve the operator type.
		 * @return The operator type associated with the token.
		 * @throws std::runtime_error if the operator type for the token is not found.
		 */
        TokenType get_operator_type(const Token& token);


		/**
		 * Retrieves the punctuation type associated with the given token.
		 *
		 * @param token [in] The token for which to retrieve the punctuation type.
		 * @return The punctuation type associated with the token.
		 * @throws std::runtime_error if the punctuation type for the token is not found.
		 */
        TokenType get_punctuation_type(const Token& token);


		/**
		 * Retrieves the identifier type associated with the given token.
		 *
		 * @param token [in] The token for which to retrieve the identifier type.
		 * @return The identifier type associated with the token.
		 * @throws std::runtime_error if the identifier type for the token is not found.
		 */
        TokenType get_identifier_type(const Token& token);


		/**
		 * Retrieves the literal type associated with the given token.
		 *
		 * @param token [in,out] The token for which to retrieve the literal type.
		 * @return The literal type associated with the token.
		 */
        TokenType get_literal_type(Token& token);


		/**
		 * Retrieves the function type associated with the given token by searching the functions map.
		 *
		 * @param token [in] The token for which to retrieve the function type.
		 * @return The function type associated with the token.
		 * @throws std::runtime_error if the function type for the token is not found.
		 */
        FunctionType get_function_type(const Token& token);


		/**
		 * Checks if the given string is a boolean literal.
		 *
		 * @param str [in] The string to check.
		 * @return `true` if the string is a boolean literal, `false` otherwise.
		 */
		inline bool is_bool_literal(const string& str);


		/**
		 * Checks if the given string is a text literal by examining the first character.
		 *
		 * @param str [in] The string to check.
		 * @return `true` if the string is a text literal (starts with a double quotation mark), `false` otherwise.
		 */
		inline bool is_text_literal(const string& str);


		/**
		 * Checks if the given token represents a function based on the functions map.
		 *
		 * @param token [in] The token to check.
		 * @return `true` if the token represents a function, `false` otherwise.
		 */
        bool is_function(const Token& token);


		/**
		 * Classifies the tokens in the given vector based on their categories.
		 *
		 * @param tokens [in,out] A reference to the vector of Token objects to classify.
		 */
        void classify(vector<Token>& tokens);


		/**
		 * Tokenizes the input string and populates the given vector with Token objects.
		 *
		 * @param tokens [in,out] A reference to the vector of Token objects to store the generated tokens.
		 * @param input [in] The input string to tokenize.
		 * @throws std::runtime_error if an invalid token is found.
		 */
		void tokenize(vector<Token>& tokens, const string& input);
	};
}

/**
 * Throws an exception indicating that an unexpected token was encountered.
 *
 * @param token [in] The unexpected token.
 * @throws std::invalid_argument with an appropriate error message.
 */
void unexpected_token(const lexer::Token& token);

/**
 * Extern declaration for the functions map.
 * The functions map maps function types to maps of function names to maps of parameter names to variable types.
 * It stores the information about available functions and their parameter types.
 */
extern map<lexer::FunctionType, map<string, map<string, lexer::VarType>>> functions;