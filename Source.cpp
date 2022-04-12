/**
*
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Ekaterina Kirilova
* @idnumber 45074
* @task 0
* @compiler VC
*
*/

#include <iostream>
#include "Helpers.h"
#include "NFA.h"
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		cout << "Not enough command line arguments.\n";
		system("pause");
		return 1;
	}
	else if (argc > 3)
	{
		cout << "Too many command line arguments.\n";
		system("pause");
		return 1;
	}
	ifstream file(argv[1], ios::in);
	if (!file)
	{
		cout << "The file can not open.\n";
		system("pause");
		return 1;
	}
	string reg_exp = argv[2];
	if (!is_RE_valid(reg_exp))
	{
		cout << "The regular expression is incorrect.\n";
		system("pause");
		return 1;
	}
	cout << "> " << base_name(argv[0]) << " " << base_name(argv[1]) << " \"" << argv[2] << "\"" << endl;
	NFA nfa = RegExp_to_NFA(reg_exp);
	//nfa.print();
	string line; int line_counter = 0;
	while (!file.eof())
	{
		getline(file, line);
		line_counter++;
		if (check_string(nfa, line))
			cout << base_name(argv[1]) << ":" << line_counter << ":" << line << endl;
	}
	file.close();
	system("pause");
	return 0;
}