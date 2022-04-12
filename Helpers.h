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
#ifndef HELPERS_H
#define HELPERS_H
#include <string>
#include "NFA.h"
#include "Dynamic_Array.h"
#include "Stack.h"
using namespace std;

/*The running time for an NFA should be O(m^n) (m is the number of states, n is the length of the input string),
because for each of the nn characters in the input string, there are up to m choices for the next state. */

string base_name(string const & str)  //I didn't write this function, I took it from the Internet
{ //It takes the file's name from the path
	return str.substr(str.find_last_of("/\\") + 1);
}

bool base_symbol(char c)
{
	if (c != '(' && c != ')' && c != '|' && c != '.' && c != '*' && c != '\\' && c>32 && c<127)
		return true;
	return false;
}

bool is_RE_valid(string RE)
{
	Stack<char> s;
	if (RE[0] == '\\') return true;
	if (RE.length() == 1 && (!base_symbol(RE[0]))) return false;
	else
	{
		for (int i = 0; RE[i] != '\0'; i++)
		{
			char c = RE[i];
			if (c < 33 || c > 126)
				return false;
			else
			{
				if (c == '(' && (base_symbol(RE[i + 1]) || RE[i + 1] == '(' || RE[i + 1] == '\\'))
					s.push(RE[i]);
				else if (c == ')' && (base_symbol(RE[i - 1]) || RE[i - 1] == '*' || RE[i - 1] == ')'))
				{
					if (s.empty())
						return false;
					else
						s.pop();
				}
				else if (c == '*' && RE[i + 1] != ')')
					return false;
				else if ((c == '|' || c == '.') && (!base_symbol(RE[i - 1]) && RE[i - 1] != ')' && RE[i-1]!='\\') && (!base_symbol(RE[i + 1]) && RE[i + 1] != '('))
					return false;
				else if (c == '\\')
				{
					if (RE[i + 1] != 's' && RE[i + 1] != 'd' && RE[i + 1] != 'a' && RE[i + 1] != 'e' && RE[i + 1] != '\\')
						return false;
					else i++;
				}
			}
		}
		return s.empty() ? true : false;
	}

}

bool check_string(NFA A, string str)
{ /* Each step requires at most s^2 computations, where s is the number of states of the NFA.
  A string of length n can be processed in time O(ns^2), and space O(s). */
	Dynamic_array<int> current;
	Dynamic_array<int> next;
	if (A.edges.size() == 0 && str.length() != 0) return false; //that's for the empty word \e
    for (int i = 0; i < A.edges.size(); i++)
	{
		if (A.edges[i].from == A.edges[0].from && A.edges[i].symbol == epsilon)
			current.push_back(A.edges[i].to);
	}
	if (current.size() == A.edges.size()) return true; //that's for the empty string ""
	else
	{
		if (current.empty()) current.push_back(A.edges[0].from);
		for (int x = 0; str[x] != '\0'; x++) //for each char in the input
		{
			char c = str[x];
			if (c >= 65 && c <= 90) //if symbol is a capital letter make it to lower to search in the nfa
				c = c + 32;
			for (int y = 0; y < current.size(); y++) //for each state in current
			{
				for (int z = 0; z < A.edges.size(); z++) //for each transition in A
				{
					if ((c == ' ' || c == '\t' || c == '\n') && current[y] == A.edges[z].from && (A.edges[z].symbol == 'S' || c == A.edges[z].symbol))
						next.push_back(A.edges[z].to);
					else if ((c>47 && c<58) && current[y] == A.edges[z].from && (A.edges[z].symbol == 'D' || c == A.edges[z].symbol))
						next.push_back(A.edges[z].to);
					else if ((c>96 && c<123) && current[y] == A.edges[z].from && (A.edges[z].symbol == 'A' || c == A.edges[z].symbol))
						next.push_back(A.edges[z].to);
					else if (current[y] == A.edges[z].from && c == A.edges[z].symbol)
						next.push_back(A.edges[z].to);
				}
			}
			current.clear();
			current = next;
			next.clear();
			for (int v = 0; v < current.size(); v++)
			{ /*If at any stage there is an output transition labelled with the empty
			  string (epsilon closure), take it without consuming any input. */
				for (int w = 0; w < A.edges.size(); w++) //for each transition in A
				{
					if (current[v] == A.edges[w].from && A.edges[w].symbol == epsilon)
						current.push_back(A.edges[w].to);
				}
			}
		}
	}
		for (int i = 0; i < current.size(); i++)
		{
			if (current[i] == A.final_state)
				return true;
		}
		return false;
}



#endif