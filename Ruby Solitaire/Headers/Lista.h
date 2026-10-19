#ifndef LISTA_H
#define LISTA_H
#pragma once

#include "Node.h"

// Lista enlazada simple con nodo centinela donde la posición 0 es el primer elemento.
template <typename E>
class Lista {
private:
	Node<E>* head;
	Node<E>* tail;
	int size;

public:
	// Crea la lista vacía con el centinela apuntado por head y tail.
	Lista() : tail(head = new Node<E>()), size(0) {}

	// Impide copiar la lista porque duplicaría los nodos sin liberarlos.
	Lista(const Lista<E>&) = delete;
	void operator = (const Lista<E>&) = delete;

	// Libera todos los nodos y el centinela de la lista.
	~Lista() {
		clear();
		delete head;
	}

	// Agrega un elemento al final de la lista.
	void append(E element) {
		tail = tail->next = new Node<E>(element);
		size++;
	}

	// Devuelve la referencia al elemento en la posición indicada.
	E& at(int pos) {
		Node<E>* n = head->next;
		for (int i = 0; i < pos; i++)
			n = n->next;
		return n->element;
	}

	// Quita el último nodo de la lista y enlaza tail con el anterior.
	void removeLast() {
		Node<E>* prev = head;
		while (prev->next != tail)
			prev = prev->next;
		delete tail;
		tail = prev;
		prev->next = nullptr;
		size--;
	}

	// Vacía la lista liberando todos sus nodos y dejando solo el centinela.
	void clear() {
		Node<E>* n = head->next;
		while (n != nullptr) {
			Node<E>* temp = n->next;
			delete n;
			n = temp;
		}
		head->next = nullptr;
		tail = head;
		size = 0;
	}

	// Devuelve la cantidad de elementos almacenados en la lista.
	int getSize() { return size; }
};

#endif
