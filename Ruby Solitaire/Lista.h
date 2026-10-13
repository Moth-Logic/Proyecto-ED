#ifndef LISTA_H
#define LISTA_H
#pragma once

#include "Node.h"

// Lista enlazada simple (del curso IC-2001). [0] es el primer elemento.
// Los métodos asumen índices válidos; el juego controla los límites.
template <typename E>
class Lista {
private:
	Node<E>* head;   // centinela
	Node<E>* tail;
	int size;

public:
	Lista() : tail(head = new Node<E>()), size(0) {}
	Lista(const Lista<E>&) = delete;
	void operator = (const Lista<E>&) = delete;
	~Lista() {
		clear();
		delete head;
	}

	void append(E element) {
		tail = tail->next = new Node<E>(element);
		size++;
	}

	E& at(int pos) {
		Node<E>* n = head->next;
		for (int i = 0; i < pos; i++)
			n = n->next;
		return n->element;
	}

	void removeLast() {
		Node<E>* prev = head;
		while (prev->next != tail)
			prev = prev->next;
		delete tail;
		tail = prev;
		prev->next = nullptr;
		size--;
	}

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

	int getSize() { return size; }
};

#endif
