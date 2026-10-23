#ifndef PILA_H
#pragma once
#define PILA_H

#include "Node.h"

// Pila enlazada simple donde el tope es el último elemento apilado.
template <typename E>
class Pila {
private:
	Node<E>* top;
	int size;

public:
	// Crea la pila vacía sin nodos en el tope.
	Pila() : top(nullptr), size(0) {}

	// Impide copiar la pila porque duplicaría los nodos sin liberarlos.
	Pila(const Pila<E>&) = delete;
	void operator = (const Pila<E>&) = delete;

	// Libera todos los nodos que quedan en la pila.
	~Pila() {
		clear();
	}

	// Apila un elemento nuevo en el tope de la pila.
	void push(E element) {
		top = new Node<E>(element, top);
		size++;
	}

	// Saca y devuelve el elemento que está en el tope.
	E pop() {
		Node<E>* n = top;
		E element = n->element;
		top = n->next;
		delete n;
		size--;
		return element;
	}

	E top() {
		if (top == nullptr) {
			throw std::runtime_error("La pila está vacía.");
		}
		return top->element;
	}

	// Vacía la pila liberando todos sus nodos.
	void clear() {
		while (size > 0)
			pop();
	}

	// Indica si la pila no tiene elementos.
	bool isEmpty() { return size == 0; }
	// Devuelve la cantidad de elementos de la pila.
	int getSize() { return size; }
};

#endif
