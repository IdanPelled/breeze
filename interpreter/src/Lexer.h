#pragma once
#include <sstream>
#include <string>
#include <vector>

#include "Token.h"


using std::string;
using std::vector;

namespace lexer
{
	class Lexer {
	public:
		Lexer(const string& row);
		vector<token::Token> lex();

	private:
		string next_word();
		bool is_operator(string& word);
		bool is_keyword(string& word);
		bool is_identifier(string& word);
		bool is_literal(string& word);


		token::Token tokenize(string& word);

		std::istringstream row;
	};
}