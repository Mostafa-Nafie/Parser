#include <bits/stdc++.h>

using namespace std;

#include "Lexer.h"
#include "Par.h"
void h_line(int n = 11) {
	for (int i = 0; i < n; i++) {
		cout << "--";
	}
}

int main() {

	Lexer L1;

	vector<pair<string, string>> tokens = L1.Tokenize("test.c");

	vector<string> lex_output;

	for (auto pr : tokens)
		if (pr.second == "id")
			lex_output.push_back("id");
		else if (pr.second == "int")
			lex_output.push_back("int");
		else if (pr.first == "int" || pr.first == "float" || pr.first == "double")
			lex_output.push_back("num_datatype");
		else
			lex_output.push_back(pr.first + "_" + pr.second);

		
	Parse p(lex_output);
	if (p.prog())
		cout << "Correct Syntax\n";
	else
		cout << "Syntax Error\n";


#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
#endif
	system("pause");
	return 0;

}
