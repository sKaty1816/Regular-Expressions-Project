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
#ifndef NFA_H
#define NFA_H
#include <iostream>
#include <string>
#include "Dynamic_array.h"
#include "Stack.h"
#include "Helpers.h"
using namespace std;
const char epsilon = '\0';

/*To build a NFA from a regular expression I use Thompson's construction - 
an algorithm which can compile regular expressions into NFAs.
https://en.wikipedia.org/wiki/Thompson's_construction
The construction time for an NFA should be O(m), where m is the number of nodes.*/

struct transition
{
	int from;
	int to;
	char symbol;
};

class NFA
{ //this class represents the nondeterministic finite automat as a graph
public:
	Dynamic_array<int> states;
	Dynamic_array<transition> edges;
	int final_state;

	NFA()
	{}

	int state_count()
	{
		return states.size();
	}

	void set_state(int v)
	{
		for (int i = 0; i < v; i++)
			states.push_back(i);
	}

	void set_edge(int from, int to, char symbol)
	{
		transition new_edge;
		new_edge.from = from;
		new_edge.to = to;
		new_edge.symbol = symbol;
		edges.push_back(new_edge);
	}

	void set_final_state(int f)
	{
		final_state = f;
	}

	/*void print()
	{
		transition new_edge;
		for (int i = 0; i < edges.size(); i++)
		{
			new_edge = edges.at(i);
			cout << "q" << edges[i].from << " --> q" << edges[i].to << " : Symbol - " << edges[i].symbol << endl;
		}
		cout << "Final state - q" << final_state << endl<<endl;
	} */
};

NFA or(NFA a, NFA b)
{ // a|b must look like this: http://hackingoff.com/images/re2nfa/2017-01-26_02-39-22_-0800-nfa.svg
	NFA result;
	int s_count = 2; //the 0th and 5th states 
	NFA med;
	transition new_edge;
	Dynamic_array<NFA> autos;
	autos.push_back(a);
	autos.push_back(b);
	s_count += a.state_count() + b.state_count();  //if the regular expression is a|b, result must have the number 
	result.set_state(s_count);                      //of states of a and b + the extra two which are 0 and 5
	//build result:
	int epsilon_edge = 1;
	for (int i = 0; i < autos.size(); i++)
	{
		result.set_edge(0, epsilon_edge, epsilon);
		med = autos.at(i);
		for (int j = 0; j < med.edges.size(); j++)
		{
			new_edge = med.edges.at(j);
			result.set_edge(new_edge.from + epsilon_edge, new_edge.to + epsilon_edge, new_edge.symbol);
		}
		epsilon_edge += med.state_count();
		result.set_edge(epsilon_edge - 1, s_count - 1, epsilon);
	}
	result.set_final_state(s_count - 1);
	return result;


}

NFA concatenation(NFA a, NFA b)
{ //http://hackingoff.com/images/re2nfa/2017-01-28_02-33-54_-0800-nfa.svg
	NFA result;
	result.set_state(a.state_count() + b.state_count() - 1); //put all of a's and b's states in result
	transition new_edge;
	for (int i = 0; i < a.edges.size(); i++) //take all of a's edges and one by one put it in result
	{
		new_edge = a.edges.at(i);
		result.set_edge(new_edge.from, new_edge.to, new_edge.symbol);
	}
	for (int i = 0; i < b.edges.size(); i++) //take all of b's edges and one by one put it in result
	{											  //add a's count 
		new_edge = b.edges.at(i);
		result.set_edge(new_edge.from + a.state_count() - 1, new_edge.to + a.state_count() - 1, new_edge.symbol);
	}
	result.set_final_state(a.state_count() + b.state_count() - 2);
	return result;
}

NFA kleene(NFA a) // NFA a looks like this http://www.cs.may.ie/staff/jpower/Courses/Previous/parsing/img5.png
{  // and the new NFA should look like this http://www.cs.may.ie/staff/jpower/Courses/Previous/parsing/img8.png 
	NFA result;
	transition new_edge;
	result.set_state(a.state_count() + 2); //a.state_count = 2 according to the first picture and the second one has two more
	result.set_edge(0, 1, epsilon); //epsilon closure
	for (int i = 0; i < a.edges.size(); i++) //take all of a's edges and one by one put it in result
	{
		new_edge = a.edges.at(i);
		result.set_edge(new_edge.from + 1, new_edge.to + 1, new_edge.symbol);
	}
	//the epsilon closures:
	result.set_edge(a.state_count(), a.state_count() + 1, epsilon);
	result.set_edge(a.state_count(), 1, epsilon);
	result.set_edge(0, a.state_count() + 1, epsilon);
	result.set_final_state(a.state_count() + 1);
	return result;
}

NFA RegExp_to_NFA(string RE)
{
	Stack<char> operators;
	Stack<NFA> operands;
	char op_symbol;
	char current_symbol;
	NFA* new_nfa;
	if (RE.empty())
	{//http://www.cs.may.ie/staff/jpower/Courses/Previous/parsing/img4.png
		new_nfa = new NFA();
		new_nfa->set_state(2);
		new_nfa->set_edge(0, 1, epsilon);
		new_nfa->set_final_state(1);
		operands.push(*new_nfa);
		delete new_nfa;
	}
	else
	{
		int counter = 0;
		for (int i = 0; RE[i] != '\0'; i++)
		{
			current_symbol = RE[i];
			if (current_symbol >= 65 && current_symbol <= 90) //all the letters in the nfa are lower case
				current_symbol = current_symbol + 32;
			if (current_symbol != '|' && current_symbol != '.' && current_symbol != '(' && current_symbol != ')' && current_symbol != '*')
			{ /*for every operand make a NFA which looks like this:
			  http://www.cs.may.ie/staff/jpower/Courses/Previous/parsing/img5.png
			  and push it in the operands stack */
				if (current_symbol == '\\' && RE[i+1] == '\\')
				{
					i++;
					current_symbol = RE[i];
				}
				else if (current_symbol == '\\' && RE[i + 1] == 'e')
				{
					new_nfa = new NFA();
					operands.push(*new_nfa);
					delete new_nfa;
					i++;
					continue;
				}
				else if (current_symbol == '\\' && RE[i + 1] == 's')
				{
					i++;
					current_symbol = 'S';
				}
				else if (current_symbol == '\\' && RE[i + 1] == 'd')
				{
					i++;
					current_symbol = 'D';
				}
				else if (current_symbol == '\\' && RE[i + 1] == 'a')
				{
					i++;
					current_symbol = 'A';
				}
				counter++;
				new_nfa = new NFA();
				new_nfa->set_state(2);
				new_nfa->set_edge(0, 1, current_symbol);
				new_nfa->set_final_state(1);
				if (!operands.empty() && counter>1)
				{
					NFA op1;
					op1 = operands.top();
					operands.pop();
					operands.push(concatenation(op1, *new_nfa));
				}
				else
					operands.push(*new_nfa);
			}
			else if (current_symbol == '|' || current_symbol == '.' || current_symbol == '(')
			{
				counter = 0;
				operators.push(current_symbol);
			}
			else if (current_symbol == '*')
			{
				counter = 0;
				NFA star = operands.top(); //take the last NFA from the operands stack, pop it
				operands.pop();            //make a kleene closure NFA from it and push it back
				operands.push(kleene(star));
			}
			else if (current_symbol == ')')
			{
				counter = 0;
				op_symbol = operators.top();
				operators.pop();
				NFA op1;
				NFA op2;
				if (op_symbol == '.')
				{
					op2 = operands.top();
					operands.pop();
					op1 = operands.top();
					operands.pop();
					operands.push(concatenation(op1, op2));
				}
				else if (op_symbol == '|')
				{
					op2 = operands.top();
					operands.pop();
					op1 = operands.top();
					operands.pop();
					operands.push(or(op1, op2));
				}
				if (operators.top() == '(') operators.pop();
			}
		}
		if (!operators.empty())
		{
			cout << "The regular expression is incorrect.\n";
			system("pause");
			exit(1);
		}
	}
	return operands.top();
}
#endif