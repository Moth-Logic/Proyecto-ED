#ifndef PILA_H
#define PILA_H

#include "Node.h"
#include <stdexcept>

using std::runtime_error;

// Pila enlazada simple donde el tope es el último elemento apilado.
template <typename E>
class Pila {
private:
	Node<E>* tope;
	int size;

public:
	// Crea la pila vacía sin nodos en el tope.
	Pila() : tope(nullptr), size(0) {}

	// Impide copiar la pila porque duplicaría los nodos sin liberarlos.
	Pila(const Pila<E>&) = delete;
	void operator = (const Pila<E>&) = delete;

	// Libera todos los nodos que quedan en la pila.
	~Pila() {
		clear();
	}

	// Apila un elemento nuevo en el tope de la pila.
	void push(E element) {
		tope = new Node<E>(element, tope);
		size++;
	}

	// Saca y devuelve el elemento que está en el tope.
	E pop() {
		if (tope == nullptr) {
			throw runtime_error("La pila está vacía.");
		}
		Node<E>* n = tope;
		E element = n->element;
		tope = n->next;
		delete n;
		size--;
		return element;
	}

	E top() {
		if (tope == nullptr) {
			throw runtime_error("La pila está vacía.");
		}
		return tope->element;
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
