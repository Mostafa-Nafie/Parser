#pragma once
#include<iostream>
#include <bits/stdc++.h>

using namespace std;

class Parse {
public:
	Parse(vector<string> x);
	bool term(string tok);
	bool idOrNum();
	bool E1();
	bool E2();
	bool E3();
	bool E();
	bool T1();
	bool T2();
	bool T3();
	bool T4();
	bool T();
	bool identify1();
	bool identify2();
	bool identify();
	bool expr1();
	bool expr2();
	bool expr3();
	bool expr4();
	bool expr5();
	bool expr();
	bool cond1();
	bool cond2();
	bool cond3();
	bool cond4();
	bool cond5();
	bool cond6();
	bool cond();
	bool prog();
	bool prog1();
	bool prog2();

	vector<string> line;
	vector<string>::iterator next_tok;
};