#include <iostream>
#include "Helpers.h"
#include "NFA.h"
#include <fstream>
#include <string>
#include "Dynamic_array.h"
using namespace std;

int main(int argc, char *argv[])
{
	cout << "> " << base_name(argv[0]) << " " << base_name(argv[1]) << " \"" << argv[2] << "\"" << endl;
	ifstream file(argv[1], ios::in);
	if (!file)
	{
		cout << "The file can not open.\n";
		system("pause");
		return 1;
	}
	//Test NFA
	NFA a, b;
	cout << "a:\n";
	a.set_state(2);
	a.set_edge(0, 1, 'a');
	a.set_final_state(1);
	a.print();
	
	cout << "b:\n";
	b.set_state(2);
	b.set_edge(0, 1, 'b');
	b.set_final_state(1);
	b.print();

	cout << "Concatenation: (a.b)\n";
	NFA ab = concatenation(a, b);
	ab.print();

	cout << "Kleene Closure: (a*)\n";
	NFA star = kleene(a);
	star.print();

	cout << "Or: (a|b)\n";
	NFA a_or_b = or(a,b);
	a_or_b.print();

	string re = "(a*)";
	string re1 = "";
	string re2 = "b";
	string re3 = "(a|b)";
	string re4 = "(a.(b.c))";
	string re5 = "((a.b).c)";
	string re6 = "((a|b).c)";
	string re7 = "((a.b)|(a.c))";
	string re8 = "(abc|(d.(e*)))";
	string re9 = "(";
	Dynamic_array<string> expressions;
	expressions.push_back(re);
	expressions.push_back(re1);
	expressions.push_back(re2);
	expressions.push_back(re3);
	expressions.push_back(re4);
	expressions.push_back(re5);
	expressions.push_back(re6);
	expressions.push_back(re7);
	expressions.push_back(re8);
	expressions.push_back(re9);
	for (int i = 0; i < 10; i++)
	{
		if (!is_RE_valid(expressions[i]))
		{
			cout << "The regular expression is incorrect.\n";
			system("pause");
			return 1;
		}
		NFA nfa = RegExp_to_NFA(expressions[i]);
		nfa.print();
		string line; int line_counter = 0;
		while (!file.eof())
		{
			getline(file, line);
			line_counter++;
			if (check_string(nfa, line))
				cout << base_name(argv[1]) << ":" << line_counter << ":" << line << endl;
		}
	}
	file.close();
	system("pause");
	return 0;
}