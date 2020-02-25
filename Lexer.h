#ifndef LEXER_H
#define LEXER_H

#include <bits/stdc++.h>
using namespace std;

class Lexer {
public:

	const set<string> keywords{ "int", "float", "double", "string", "char",
		"void", "bool", "if", "then", "else", "for", "while", "do", "break",
		"continue", "main", "return", "cin", "cout", "printf", "#include" };
	const set<string> operators{ "<", ">", "<=", ">=", "*", "+", "-", "/", "=",
		"-=", "*=", "+=", "/=", "++", "--", "==", "!=", "&", "|", "^" };
	const set<string> delimiters{ "{", "}", ",", "(", ")", ";", "[", "]", "." };
	const set<char> whitespace{ ' ', '\n', '\t', 13 };


	//	Lexer();
	//virtual ~Lexer();
	bool isIdentifier(const string &str);
	bool isInteger(const string &str);
	bool isKeyword(const string &str);
	bool isOperator(const string &str);
	bool isDelimiter(const string &str);
	bool isNotLegal(const string &str);
	void printRoleOfToken(const string &token);
	vector<string> getClasses(const string &str);
	vector<pair<string, string>> Tokenize(const string &nameOfFile);

	//private:
	//const vector<string> keywords;
};

#endif // LEXER_H
