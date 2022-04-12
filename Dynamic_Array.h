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
#ifndef DYNAMIC_ARRAY
#define DYNAMIC_ARRAY
#include <cassert>
#include <stddef.h>

/*I use the dynamic array structure to have an easy access to every element. In the NFA class I use one 
dynamic array for the transitions and one for the states. I also use it in the check_string() function. */

template <class T>
class Dynamic_array
{
private:
	T* data_;
	int size_;
	int capacity_;

	void init_();
	void clear_();
	void copy_(const Dynamic_array<T>& other);
	void reserve_(int capacity);
	void erase_(int first, int last);

public:
	Dynamic_array();
	//copy constructor
	Dynamic_array(const Dynamic_array<T>& other);
	//assignment operator
	Dynamic_array<T>& operator=(const Dynamic_array<T>& other);
	~Dynamic_array();

	int size();
	bool empty() const;
	T& front() const;
	T& at(int index) const;
	T& operator[](int index) const;
	void push_back(const T& value);
	void pop_back();
	void clear();

};

//private methods
template <class T>
void Dynamic_array<T>::init_()
{
	data_ = NULL;
	size_ = capacity_ = 0;
}

template <class T>
void Dynamic_array<T>::clear_()
{
	delete[] data_;
	data_ = NULL;
	size_ = capacity_ = 0;
}

template <class T>
void Dynamic_array<T>::copy_(const Dynamic_array<T>& other)
{
	clear_();
	reserve_(other.size_);
	size_ = other.size_;
	for (int i = 0; i < size_; i++)
		data_[i] = other.data_[i];
}

template<class T>
void Dynamic_array<T>::reserve_(int capacity)
{
	capacity = (capacity <= 0 ? 1 : capacity);
	if (capacity <= capacity_) return;
	T* data = new T[capacity];
	capacity_ = capacity;
	for (int i = 0; i < size_; i++)
		data[i] = data_[i];
	delete[] data_;
	data_ = data;
}

template <class T>
void Dynamic_array<T>::erase_(int first, int last)
{
	first = (first < 0 ? 0 : first);
	last = (last < 0 ? first + 1 : last);
	if (first >= size_ || last <= first)
		return;
	for (int i = last; i < size_; i++)
		data_[first + i - last] = data_[i];
	size_ -= last - first;
}

//public methods
template<class T>
Dynamic_array<T>::Dynamic_array()
{
	init_();
}

template<class T>
Dynamic_array<T>::Dynamic_array(const Dynamic_array<T>& other)
{
	init_();
	copy_(other);
}

template<class T>
Dynamic_array<T>& Dynamic_array<T>::operator=(const Dynamic_array<T>& other)
{
	if (this != &other)
		copy_(other);
	return *this;
}

template<class T>
Dynamic_array<T>::~Dynamic_array()
{
	clear_();
}

template<class T>
int Dynamic_array<T>::size()
{
	return size_;
}

template<class T>
bool Dynamic_array<T>::empty() const
{
	return !size_;
}

template<class T>
T& Dynamic_array<T>::front() const
{
	assert(size_ > 0);
	return data_[0];
}

template<class T>
T& Dynamic_array<T>::at(int index) const
{
	assert(index >= 0 && index < size_);
	return data_[index];
}

template<class T>
T& Dynamic_array<T>::operator[](int index) const
{
	assert(index >= 0 && index < size_);
	return data_[index];
}

template<class T>
void Dynamic_array<T>::push_back(const T& value)
{
	if (size_ >= capacity_)
		reserve_(capacity_ * 2);
	data_[size_] = value;
	size_++;
}


template<class T>
void Dynamic_array<T>::pop_back()
{
	erase_(size_ - 1, size_);
}


template<class T>
void Dynamic_array<T>::clear()
{
	clear_();
}


#endif