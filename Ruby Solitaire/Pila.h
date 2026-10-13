#ifndef PILA_H
#pragma once
#define PILA_H

#include "Node.h"

// Pila enlazada simple (del curso IC-2001): top es el último elemento apilado.
template <typename E>
class Pila {
private:
	Node<E>* top;
	int size;

public:
	Pila() : top(nullptr), size(0) {}
	Pila(const Pila<E>&) = delete;
	void operator = (const Pila<E>&) = delete;
	~Pila() {
		clear();
	}

	void push(E element) {
		top = new Node<E>(element, top);
		size++;
	}

	E pop() {
		Node<E>* n = top;
		E element = n->element;
		top = n->next;
		delete n;
		size--;
		return element;
	}

	void clear() {
		while (size > 0)
			pop();
	}

	bool isEmpty() { return size == 0; }
	int getSize() { return size; }
};

#endif
