#pragma once
#include "declaration.h"

template<typename T>
struct LinkedStack_Node {
	T data;
	struct LinkedStack_Node *next;
};

// ´øÎ²Ö¸ÕëµÄÁ´Õ»
template<typename T>
class Printable_Stack
{
	typedef struct LinkedStack_Node<T> LSNode, *LinkedStack;
public:
	Printable_Stack();
	bool push(T e);
	bool pop(T &e);
	bool getTop(T &e);
	bool isEmpty();
	void print();
	vector<T> getAllElems();
	void destroy();

private:
	LinkedStack LS;
};

template<typename T>
inline Printable_Stack<T>::Printable_Stack()
{
	this->LS = NULL;
}

template <typename T>
bool Printable_Stack<T>::push(T e)
{
	if (this->isEmpty())
	{
		LS = (LSNode *)malloc(sizeof(LSNode));
		LS->data = e;
		LS->next = NULL;
		return true;
	}
	LSNode *p = (LSNode *)malloc(sizeof(LSNode));
	if (p == NULL)
	{
		return false;
	}
	p->data = e;
	p->next = LS;
	LS = p;
	return true;
}

template <typename T>
bool Printable_Stack<T>::pop(T &e)
{
	if (this->isEmpty())
	{
		return false;
	}
	LSNode *p = LS;
	LS = LS->next;
	e = p->data;
	free(p);
	return true;
}

template <typename T>
bool Printable_Stack<T>::getTop(T & e)
{
	if (this->isEmpty())
	{
		return false;
	}
	e = LS->data;
	return true;
}

template <typename T>
bool Printable_Stack<T>::isEmpty()
{
	if (LS == NULL)
	{
		return true;
	}
	return false;
}

template <typename T>
void Printable_Stack<T>::print()
{
	LSNode *p = LS;
	while (p != NULL)
	{
		cout << p->data << ' ';
		p = p->next;
	}
	cout << endl;
}

template <typename T>
vector<T> Printable_Stack<T>::getAllElems()
{
	vector<T> output;
	LSNode *p = LS;
	while (p != NULL)
	{
		output.push_back(p->data);
		p = p->next;
	}
	return output;
}

template <typename T>
void Printable_Stack<T>::destroy()
{
	while (LS != NULL)
	{
		LS = LS->next;
	}
}