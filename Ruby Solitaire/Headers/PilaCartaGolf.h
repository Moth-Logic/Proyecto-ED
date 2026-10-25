#ifndef PILACARTAGOLF_H
#define PILACARTAGOLF_H

#include "Carta.h"
#include "Lista.h"

using std::string;

// Una columna de la mesa del Golf (hay 7). [0] = base de la columna, último = tope.
class PilaCartaGolf {
private:
	Lista<Carta> cartas;

public:
	// Agrega una carta encima de la columna.
	void push(const Carta& carta) { cartas.append(carta); }

	// Quita y devuelve la carta del tope de la columna.
	Carta pop() {
		Carta carta = tope();
		if (!vacia())
			cartas.removeLast();
		return carta;
	}

	// Devuelve la carta de la posición indicada tomando la base como 0.
	Carta operator[](int indice) { return cartas.at(indice); }
	// Devuelve la carta del tope o una carta vacía si la columna no tiene cartas.
	Carta tope() { return vacia() ? Carta() : (*this)[tamano() - 1]; }
	// Indica si la columna está sin cartas.
	bool vacia() { return tamano() == 0; }
	// Devuelve la cantidad de cartas de la columna.
	int tamano() { return cartas.getSize(); }

	// Voltea el tope solo cuando esa carta está boca abajo.
	void voltearTope() {
		if (!vacia() && !(*this)[tamano() - 1].estaBocaArriba())
			cartas.at(tamano() - 1).voltear();
	}

	// Deja la columna sin cartas.
	void limpiar() { cartas.clear(); }

	// Arma el texto de la columna en una sola línea: "K-E 7-E 3-T".
	// Las cartas boca abajo se muestran como "??".
	string mostrar() {
		string texto = "";
		for (int i = 0; i < tamano(); i++) {
			if (i > 0)
				texto += " ";
			Carta carta = (*this)[i];
			texto += carta.estaBocaArriba() ? carta.toString() : "??";
		}
		return texto;
	}
};

#endif
