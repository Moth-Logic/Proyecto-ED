#pragma once

// Nodo genérico con un dato y un puntero al siguiente, base de Lista y Pila.
template <typename E>
class Node {
public:
	E element;
	Node<E>* next;

	// Crea un nodo con su elemento y el enlace al nodo siguiente.
	Node(E element, Node<E>* next = nullptr) : element(element), next(next) {}
	Node() : next(nullptr) {}
};
