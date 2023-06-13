#pragma once
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>


#include "Token.h"

using std::vector;

extern map<string, token::token_t> keywords_map;
extern map<string, token::token_t> operators_map;
extern map<token::TokenType, map<string, map<string, token::TokenType>>> functions;


namespace lexer
{
	class Lexer {
	public:
		Lexer(const string& row);
		const vector<token::Token> lex();

	private:
		const string next_word();
		bool is_operator(const string& word);
		bool is_keyword(const string& word);
		bool is_function(const string& word);
		bool is_identifier(const string& word);
		bool is_literal(const string& word);
		
		token::token_t get_function_type(const string& func);

		token::Token tokenize(const string& word);

		std::istringstream row;
	};
}