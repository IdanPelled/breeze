#pragma once
#include <string>
#include <map>
#include <iostream>

using std::string;
using std::map;

namespace token
{
	typedef enum class TokenType {
		// KeyWords:
		OPEN,
		CLOSE,
		F_OPEN,
		F_CLOSE,
		WHEN,
		DO,
		OTHERWISE,
		YES,
		NO,
		COMMA,

		// Functions
		ACTION_FUNCTION,
		VALUE_FUNCTION,
		// ToInt, ToString

		// Identifiers:
		IDENTI,

		// Literals:
		TEXT,
		NUMBER,
		BOOL,

		// Operators
		SET,
		TO,
		PLUS,
		MINUS,
		MULTIPLY,
		EQUAL,
		GREATER,
		SMALLER,

		// No a token
		END_OF_TOKENS,

	} token_t;

	class Token {
	public:
		Token(token_t type);
		Token();

		token_t get_type() const;
		string get_data(const string& key) const;
		bool operator == (Token& t);
		bool operator < (Token& t);
		bool operator > (Token& t);

	protected:
		token_t type;
		map<string, string> data;
	};

	class KeyWord : public Token {
	public:
		KeyWord(){}
		KeyWord(token_t type);
	};

	class Identifier : public Token {
	public:
		Identifier(){}
		Identifier(const string& name);
	};

	class Literal : public Token {
	public:
		Literal(const string& value);
		Literal(){}
	};

	class Operator : public Token {
	public:
		Operator(){}
		Operator(token_t type);
	};

	class Function : public Token {
	public:
		Function(){}
		Function(const string& name, token_t type);
	};
}