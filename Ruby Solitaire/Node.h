#pragma once

template <typename E>
class Node {
public:
	E element;
	Node<E>* next;

	Node(E element, Node<E>* next = nullptr) : element(element), next(next) {}
	Node() : next(nullptr) {}
};
