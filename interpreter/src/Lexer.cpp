#include <map>

#include "Lexer.h"

using std::map;
using std::string;

using namespace lexer;

map<string, token::token_t> keywords_map = {
	{"{", token::TokenType::OPEN},
	{"}", token::TokenType::CLOSE},
	{"when", token::TokenType::WHEN},
	{"otherwise", token::TokenType::OTHERWISE},
};

map<string, token::token_t> operators_map = {
	{"=", token::TokenType::ASSIGNMENT},
	{"+", token::TokenType::PLUS},
	{"-", token::TokenType::MINUS},
	{"*", token::TokenType::MULTIPLY},
	{"==", token::TokenType::EQUAL},
	{">", token::TokenType::GREATER},
	{"<", token::TokenType::SMALLER},

};

Lexer::Lexer(const string& row) : row{ std::istringstream(row) }
{
}

const string& Lexer::next_word() {
	string word;
	row >> word;
	return word;
}

inline bool is_in_map(const string& key, const map<string, token::token_t>& dict) {
	return dict.find(key) != dict.end();
}

bool Lexer::is_operator(const string& word) {
	return is_in_map(word, operators_map);
}

bool Lexer::is_keyword(const string& word) {
	return is_in_map(word, keywords_map);
}

bool Lexer::is_identifier(const string& word) {
	return isalpha(word[0]);
}

bool Lexer::is_literal(const string& word) {
	return (
		word == "yes"
		|| word == "no"
		|| word[0] == '\"'
		|| (isdigit(word[0])
		|| word[0] == '-')
	);
}

token::Token Lexer::tokenize(const string& word) {
	if (is_keyword(word))
		return token::KeyWord(keywords_map[word]);

	if (is_operator(word))
		return token::Operator(operators_map[word]);

	if (is_literal(word))
		return token::Literal(word);

	if (is_identifier(word))
		return token::Identifier(word);

	throw std::invalid_argument("Undifined token: " + word);
}

const vector<token::Token> Lexer::lex()
{
	string word;
	vector<token::Token> tokens;
	
	while ((word = next_word()) != "")
		tokens.push_back(tokenize(word));
	
	return tokens;
}