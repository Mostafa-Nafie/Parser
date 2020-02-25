#include "Par.h"

Parse::Parse(vector<string> x)
{
	line = x;
	next_tok = line.begin();
	for (vector<string>::iterator i = line.begin(); i != line.end(); i++)
		cout << *i << endl;
}

bool Parse::term(string tok)
{
	//If we reached the end of the line vector and the expression we are trying to match is not over yet 
	//we will reach the end iterator and we using dereference operator * on it will cause an error
	//prevents errors in the program when the parser is checking an incomplete (wrong) syntax like x = 2 (no ;)
	if (next_tok != line.end())
		return *next_tok++ == tok;
	else
		return false;
}



//numbers and identifiers both come next interchangeably after the equal (=) notation so this function checks weather if one of them exist
bool Parse::idOrNum()
{
	std::vector<string>::iterator save = next_tok;
	if (term("id"))
		return true;
	else
	{
		next_tok = save;
		return term("int");
	}
}



//E --> T+E | T-E | T 
bool Parse::E1() { return T() && term("+_op") && E(); }
bool Parse::E2() { return T() && term("-_op") && E(); }
bool Parse::E3() { return T(); }

bool Parse::E() {
	std::vector<string>::iterator save = next_tok;
	if (E1())
		return true;
	else
	{
		next_tok = save;
		if (E2())
			return true;
		else
		{
			next_tok = save;
			return E3();
		}
	}
}


//T --> int*T | int/T | int | id*T | id/T | id | (E)
bool Parse::T1() { return idOrNum() && term("*_op") && T(); }
bool Parse::T2() { return idOrNum() && term("/_op") && T(); }
bool Parse::T3() { return idOrNum(); }
bool Parse::T4() { return term("(_del") && E() && term(")_del"); }
bool Parse::T() {
	vector<string>::iterator save = next_tok;
	if (T1())
		return true;
	else
	{
		next_tok = save;
		if (T2())
			return true;
		else
		{
			next_tok = save;
			if (T3())
				return true;
			else {
				next_tok = save;
				return T4();
			}
		}
	}
}

//cond --> E == E | E != E | E >= E | E <= E | E > E | E < E
bool Parse::cond1() { return E() && term("==_op") && E(); }
bool Parse::cond2() { return E() && term(">=_op") && E(); }
bool Parse::cond3() { return E() && term("<=_op") && E(); }
bool Parse::cond4() { return E() && term("!=_op") && E(); }
bool Parse::cond5() { return E() && term(">_op") && E(); }
bool Parse::cond6() { return E() && term("<_op") && E(); }
bool Parse::cond() {
	vector<string>::iterator save = next_tok;
	if (cond1())
		return true;
	else
	{
		next_tok = save;
		if (cond2())
			return true;
		else
		{
			next_tok = save;
			if (cond3())
				return true;
			else {
				next_tok = save;
				if (cond4())
					return true;
				else
				{
					next_tok = save;
					if (cond5())
						return true;
					else {
						next_tok = save;
						return cond6();
					}
				}
			}
		}
	}
}


//used for identifiers like int x; | int x = 2 * 3;
bool Parse::identify1() { return term("num_datatype") && term("id") && term("=_op") && E() && term(";_del"); }
bool Parse::identify2() { return term("num_datatype") && term("id") && term(";_del"); }
bool Parse::identify()
{
	vector<string>::iterator save = next_tok;
	if (identify1())
		return true;
	else
	{
		next_tok = save;
		return(identify2());
	}
}


//expr --> if (COND) EXPR else EXPR | while (COND) EXPR | id = EXPR
bool Parse::expr1() { return identify(); };
bool Parse::expr2() { return term("id") && term("=_op") && E() && term(";_del"); }
bool Parse::expr3() { return term("if_key") && term("(_del") && cond() && term(")_del") && expr() && term("else_key") && expr(); }
bool Parse::expr4() { return term("while_key") && term("(_del") && cond() && term(")_del") && expr(); }
bool Parse::expr5() { return term("for_key") && term("(_del") && identify() && cond() && term(";_del") && term("id") && term("=_op") && E() && term(")_del") && expr(); }
bool Parse::expr() {
	vector<string>::iterator save = next_tok;
	if (expr1())
		return true;
	else
	{
		next_tok = save;
		if (expr2())
			return true;
		else
		{
			next_tok = save;
			if (expr3())
				return true;
			else {
				next_tok = save;
				if (expr4())
					return true;
				else {
					next_tok = save;
					return expr5();
				}
			}
		}
	}
}


//This is the starting grammar so after it's finished if must check if it reached the end of the tokens
//IT makes the compiler support multiple statements
// prog --> prog expr | expr
bool Parse::prog1() { return expr() && prog(); }
bool Parse::prog2() { return expr(); }
bool Parse::prog() {
	vector<string>::iterator save = next_tok;
	if (prog1())
		return true;
	else
	{
		next_tok = save;
		return (prog2() && next_tok == line.end());  //after making the last derivation, it checks if it reached the end of the token
													 //if not then there are other statments that haven't been checked yet
	}
}
