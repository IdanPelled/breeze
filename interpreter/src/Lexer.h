#pragma once
#include <sstream>
#include <string>
#include <vector>

#include "Token.h"

using std::vector;

namespace lexer
{
	class Lexer {
	public:
		Lexer(const string& row);
		const vector<token::Token> lex();

	private:
		const string& next_word();
		bool is_operator(const string& word);
		bool is_keyword(const string& word);
		bool is_identifier(const string& word);
		bool is_literal(const string& word);


		token::Token tokenize(const string& word);

		std::istringstream row;
	};
}