#ifndef PILACARTAGOLF_H
#define PILACARTAGOLF_H

#include "Carta.h"
#include "Lista.h"

using std::string;
using std::to_string;

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
		int tamanoBocaArriba = tamano();

		if (vacia())
			return "";

		string texto = "";
		for (int i = 0; i < tamano(); i++) {
			Carta carta = (*this)[i];

			if (i > 0) {
				texto += "\n";
			}
			if (carta.estaBocaArriba() && i < tamano() - 1) {
				texto += dibujarCarta(carta, false);
				tamanoBocaArriba--;
			}
			else {
				texto += dibujarCarta(carta, true);
				tamanoBocaArriba--;
			}
		}
		return texto;
	}

	// Dibuja las cartas que no están ocultas.
	string dibujarCarta(Carta carta, bool completa) {
		string palo = "";
		string color = "";

		switch (carta.getPalo()) {
		case 'E':
			palo = "♠";
			color = "\033[30m";
			break;

		case 'D':
			palo = "♦";
			color = "\033[31m";
			break;

		case 'T':
			palo = "♣";
			color = "\033[30m";
			break;

		case 'C':
			palo = "♥";
			color = "\033[31m";
			break;

		default:
			palo = "";
			color = "";
			break;
		}

		string cartaTexto = carta.toString();
		string valor = cartaTexto.substr(0, cartaTexto.find('-'));

		string texto = "┌───────┐\n";

		texto += "│\033[47m" + color + valor;
		for (int i = 0; i < 7 - valor.length(); i++)
			texto += " ";
		texto += "\033[0m│";

		if (completa) {
			texto += "\n│\033[47m       \033[0m│\n";
			texto += "│\033[47m   " + color + palo + "   \033[0m│\n";
			texto += "│\033[47m       \033[0m│\n";

			texto += "│\033[47m";
			for (int i = 0; i < 7 - valor.length(); i++)
				texto += " ";
			texto += color + valor + "\033[0m│\n";

			texto += "└───────┘";
		}
		return texto;
	}
};

#endif
