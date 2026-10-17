#ifndef PILACARTASPIDER_H
#define PILACARTASPIDER_H

#include "Carta.h"
#include "Lista.h"

using std::string;

// Una columna de la mesa (hay 10). [0] = base de la columna, último = tope.
class PilaCartaSpider {
private:
	Lista<Carta> cartas;

public:
	void push(const Carta& carta) { cartas.append(carta); }

	Carta operator[](int indice) { return cartas.at(indice); }
	Carta tope() { return vacia() ? Carta() : (*this)[tamano() - 1]; }
	bool vacia() { return tamano() == 0; }
	int tamano() { return cartas.getSize(); }

	// Voltea el tope solo si está boca abajo.
	void voltearTope() {
		if (!vacia() && !(*this)[tamano() - 1].estaBocaArriba())
			cartas.at(tamano() - 1).voltear();
	}

	void limpiar() { cartas.clear(); }

	// Quita las últimas 'cantidad' cartas y las devuelve en orden base->tope.
	// Devuelve puntero con new: el que llama debe hacer delete.
	Lista<Carta>* extraerGrupo(int cantidad) {
		Lista<Carta>* grupo = new Lista<Carta>();
		for (int i = tamano() - cantidad; i < tamano(); i++)
			grupo->append((*this)[i]);
		for (int i = 0; i < cantidad; i++)
			cartas.removeLast();
		return grupo;
	}

	// true si desde 'desde' hasta el tope todas las cartas están boca arriba
	// y cada una vale 1 menos que la anterior leyendo hacia el tope.
	bool formaEscaleraAscendente(int desde) {
		for (int i = desde; i < tamano(); i++) {
			if (!(*this)[i].estaBocaArriba())
				return false;
			if (i > desde && (*this)[i].getValor() != (*this)[i - 1].getValor() - 1)
				return false;
		}
		return true;
	}

	// Escalera completa K->A del mismo palo, boca arriba (se elimina del juego).
	bool esEscaleraCompleta() {
		if (tamano() != 13 || (*this)[0].getValor() != 13)
			return false;
		for (int i = 1; i < 13; i++)
			if ((*this)[i].getPalo() != (*this)[0].getPalo())
				return false;
		return formaEscaleraAscendente(0);
	}

	// Quita las 13 cartas de la escalera completa si la hay.
	void extraerEscaleraCompleta() {
		if (esEscaleraCompleta())
			for (int i = 0; i < 13; i++)
				cartas.removeLast();
	}

	// Muestra las cartas ocultas y llama a otra función si no es el caso.
	string mostrar() {
		if (vacia())
			return "";

		string texto = "";
		for (int i = 0; i < tamano(); i++) {
			Carta carta = (*this)[i];

			if (i > 0) {
				texto += "\n";
			}
			if (!carta.estaBocaArriba()) {
				texto += "┌───────┐\n";
				texto += "│";
				texto += "\033[45m░░░░░░░";
				texto += "\033[0m│";
			}
			else if (carta.estaBocaArriba() && i < tamano() - 1) {
				texto += dibujarCarta(carta, false);
			}
			else {
				texto += dibujarCarta(carta, true);
			}
		}
		return texto;
	}

	// Dibuja las cartas que no están ocultas.
	string dibujarCarta(Carta carta, bool completa) {
		string palo = (carta.getPalo() == 'E') ? "♠" : string(1, carta.getPalo());

		string cartaTexto = carta.toString();
		string valor = cartaTexto.substr(0, cartaTexto.find('-'));

		string texto = "┌───────┐\n";

		texto += "│" + valor;
		for (int i = 0; i < 7 - valor.length(); i++)
			texto += " ";
		texto += "│";

		if (completa) {
			texto += "\n│       │\n";
			texto += "│   " + palo + "   │\n";
			texto += "│       │\n";

			texto += "│";
			for (int i = 0; i < 7 - valor.length(); i++)
				texto += " ";
			texto += valor + "│\n";

			texto += "└───────┘";
		}
		return texto;
	}
};

#endif
