#include "Lexer.h"

#define present(a, x) (find(a.begin(), a.end(), x) != a.end())


bool Lexer::isIdentifier(const string &str) {

	// Identifiers only start with an alphabet or underscore (_)
	if (!(str[0] == '_' || isalpha(str[0]))) //str[0]<'a' || str[0]>'z' || str[0]<'A' || str[0]>'Z'
		return false;

	// Check for any violations(Is there any char that's not an alpha or num or _ ? )
	for (int i = 1; i < (int)str.length(); i++) {
		if (!(str[i] == '_' || isalnum(str[i])))
			return false;

	}
	// The given string is a valid identifier
	return true;
}

bool Lexer::isInteger(const string &str) {
	// Check if the whole given sting is numeric
	for (int i = 0; i < (int)str.length(); i++) {
		if (!isdigit(str[i]))
			return false;
	}
	// The given string is numeric
	return true;
}

bool Lexer::isKeyword(const string &str) { return present(keywords, str); }
bool Lexer::isOperator(const string &str) { return present(operators, str); }
bool Lexer::isDelimiter(const string &str) { return present(delimiters, str); }

bool Lexer::isNotLegal(const string &str)
{
	return str == " " || str == "\n";
}


void Lexer::printRoleOfToken(const string &token) {

	if (isOperator(token))
		//cout << "(Operator: " << token << ")\n";
		cout << token << setw(23 - token.length()) << "Operator\n";
	else if (isDelimiter(token))
		cout << token << setw(23 - token.length()) << "Delimiter\n";
	//cout << "(Delimiter: " << token << ")\n";
	else if (isKeyword(token))
		cout << token << setw(23 - token.length()) << "Keyword\n";
	//cout << "(Keyword: " << token << ")\n";
	else if (isIdentifier(token))
		cout << token << setw(23 - token.length()) << "Identifier\n";
	//cout << "(Identifier: " << token << ")\n";
	else
		cout << token << setw(23 - token.length()) << "Integer\n";
	//cout << "(Digit: " << token << ")\n";

}

vector<string> Lexer::getClasses(const string &str) {

	vector<string> ret;

	//the first, the higher priority

	if (isOperator(str))
		ret.push_back("op");

	if (isDelimiter(str))
		ret.push_back("del");

	if (isInteger(str))
		ret.push_back("int");

	if (isKeyword(str))
		ret.push_back("key");

	if (isIdentifier(str))
		ret.push_back("id");

	return ret;
}

void syntax_error() {
	cout << "Syntax error\n";
	exit(0);
}

// set of possible token classes:
// id : identifier, int : integer, key : keyword, op : operator
// del : delimiter, str : string, com : "comment"


//stores the text in the file into a string src then starts to look for the type of the first char in the string then it extends it and
//add another char to it and check if the new string has a valid type or not. if yes, then it try to extend the string even more until it
//meets a string that has no valid type, then it moves back and add the string that is valid to the tokens and makes lexeme empty to 
//take the first char of the following string that's still remaining
vector<pair<string, string>> Lexer::Tokenize(const string &testFile) {

	ifstream ifs("test.c");				//creates a variable of type inputFileStream

	char c = ifs.get();					//reads the first char in the file

	string src = "";

	while (ifs.good()) {				//good = not the EOF
		src += c;						//reads all chars from the file and store them in src 
		c = ifs.get();
	}

	ifs.close();

	vector<pair<string, string>> tokens;

	vector<string> candidateClasses, nextCandidateClasses;

	string lexeme = "";


	//for loop on all the charcters in the string src
	for (int i = 0; i < (int)src.size(); ++i) {

		//if the current char is a whitespace, then checks if the lexeme is empty or not -> if it's empty then nothing happens(nothing has
		//been read yet. if it's not empty then it checks the candidateClasses vector if it's empty then there's an error if not then 
		//it takes the previous the string before the space(lexeme) and makes a pair of it and its type (0) then push it back into the vector of
		//tokens and makes lexeme empty to take a new string
		if (present(whitespace, src[i])) {

			if (!lexeme.empty()) {

				if (candidateClasses.empty())
					syntax_error();

				tokens.push_back(make_pair(lexeme, candidateClasses[0]));

				lexeme = "";

			}

			continue;		//if we meet a whitespace we skip it and look for what is after it
		}


		//checks for comments that strats with /* symbol
		//if there were no lexeme that has not been stored yet when we meet a comment then there's an error
		if (i + 1 != (int)src.size() && src[i] == '/' && src[i + 1] == '*') {

			if (!lexeme.empty())
				syntax_error();

			bool good = false;			//returns false if we didn't meet the */ symbol again

			//starts from after the * symbol (i+1) character and then starts to pass the characters until it meets a * symbol again
			for (int j = i + 2; j < (int)src.size(); ++j)

				//if there's a * symbol in the character after the one the lexer is reading then it stores the substring from 
				//the first / symbol until the second / symbol and push it into the tokens vector as a comment and then moves the 
				//char the lex is reading to the char after the * symbol
				if (j + 1 != (int)src.size() && src[j] == '*' && src[j + 1] == '/') {
					tokens.push_back(make_pair(src.substr(i, j - i + 2), "com"));
					i = j + 1;
					good = true;
					break;
				}

			if (!good)
				syntax_error();

			continue;
		}
		
		//checks if the char the lex is reading is the start of a string
		if (src[i] == '\"') {

			if (!lexeme.empty())
				syntax_error();

			bool good = false;		//returns false if we didn't meet the ending " symbol


			//pushes the substring between the " " symbols into the tokens vector with the type str and moves the reading cursor to the 
			//char after the ending " symbol
			for (int j = i + 1; j < (int)src.size(); ++j)
				if (src[j] == '\"') {
					tokens.push_back(make_pair(src.substr(i, j - i + 1), "str"));
					i = j;
					good = true;
					break;
				}

			if (!good)
				syntax_error();

			continue;

		}

		lexeme += src[i];			//if the char we are reading is not a whitespace or the start of a comment or a string then add it to 
									// the string lexeme

		candidateClasses = getClasses(lexeme);				//pushes the type of the string until the read char into the candidateClasses

		//if we are at the last charcter of the file
		if (i + 1 == (int)src.size()) {

			if (candidateClasses.empty())
				syntax_error();

			tokens.push_back(make_pair(lexeme, candidateClasses[0]));		//adds the last lexeme to the vector of tokens

		}
		else {

			//somekind of lookahead. it checks if we add another char to the lexeme we have, would it have a valid type or not?

			lexeme += src[i + 1];											//reads the next charcter

			nextCandidateClasses = getClasses(lexeme);						//checks if there's a valid type for the string read until now
			//if no nothing is pushed into the vector
			//if yes this type is pushed into the vector and then checks if we add another charcter would we be able to 

			lexeme.pop_back();		//remove the added char from the string lexeme

			//after we add another char to the lexeme if there are no possible type for it then nextCandidateClasses becomes
			//empty otherewise it's not
			if (!candidateClasses.empty() && nextCandidateClasses.empty()) {

				tokens.push_back(make_pair(lexeme, candidateClasses[0]));

				lexeme = "";		//make it empty to take a new lexeme

			}
		}

	}

	return tokens;
}
