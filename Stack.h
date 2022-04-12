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
#ifndef STACK_H
#define STACK_H
const int MS= 20;

/*I used the stack structure first in the function which checks if a regular expression is valid 
(if the number of '(' matches the number ')').
I also use it when I build a NFA from a regular expression. 
Push in different stacks the operators and the operands.*/

template <class T = int>
class Stack
{
private:
	int stack_size;
	int stack_top;
	T *stack_arr;
public:
	Stack(int = MS);
	~Stack();
	bool empty();
	bool full();
	void push(const T&);
	void pop();
	T& top();
	int size() const;
};

template <class T>
Stack<T>::Stack(int size)
{
	stack_size = (size <= 0) ? MS : size;
	stack_top = -1;
	stack_arr = new T[stack_size];
}

template<class T>
Stack<T>::~Stack()
{
	delete[] stack_arr;
}

template<class T>
bool Stack<T>::empty()
{
	return (stack_top == -1);
}

template<class T>
bool Stack<T>::full()
{
	return(stack_top == stack_size - 1);
}

template<class T>
void Stack<T>::push(const T& item)
{
	if (!full())
		stack_arr[++stack_top] = item;
	else
	{
		cout << "The stack is full.\n";
		exit(1);
	}
}

template<class T>
void Stack<T>::pop()
{
	if (!empty())
		stack_arr[stack_top--];
	else
	{
		cout << "The stack is empty.\n";
		exit(1);
	}
}

template<class T>
T& Stack<T>::top()
{
	return stack_arr[stack_top];
}

template<class T>
int Stack<T>::size() const
{
	return stack_top + 1;
}

#endif