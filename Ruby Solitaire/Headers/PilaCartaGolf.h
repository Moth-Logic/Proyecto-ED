#ifndef PILACARTASPIDER_H
#define PILACARTASPIDER_H

#include "Carta.h"
#include "Lista.h"

using std::string;
using std::to_string;

// Representa una de las 10 columnas de la mesa, con la base en la posición 0 y el tope al final.
class PilaCartaSpider {
private:
	Lista<Carta> cartas;

public:
	// Agrega una carta encima de la columna.
	void push(const Carta& carta) { cartas.append(carta); }

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

	// Quita las últimas 'cantidad' cartas y las devuelve en una lista nueva que el llamador debe liberar.
	Lista<Carta>* extraerGrupo(int cantidad) {
		Lista<Carta>* grupo = new Lista<Carta>();
		for (int i = tamano() - cantidad; i < tamano(); i++)
			grupo->append((*this)[i]);
		for (int i = 0; i < cantidad; i++)
			cartas.removeLast();
		return grupo;
	}

	// Indica si desde la carta 'desde' hasta el tope todas están boca arriba y bajan de a uno en valor.
	bool formaEscaleraAscendente(int desde) {
		for (int i = desde; i < tamano(); i++) {
			if (!(*this)[i].estaBocaArriba())
				return false;
			if (i > desde && (*this)[i].getValor() != (*this)[i - 1].getValor() - 1)
				return false;
		}
		return true;
	}

	// Indica si la columna completa es una escalera K->A del mismo palo y boca arriba.
	bool esEscaleraCompleta() {
		if (tamano() != 13 || (*this)[0].getValor() != 13)
			return false;
		for (int i = 1; i < 13; i++)
			if ((*this)[i].getPalo() != (*this)[0].getPalo())
				return false;
		return formaEscaleraAscendente(0);
	}

	// Quita las 13 cartas de la escalera completa cuando existe.
	void extraerEscaleraCompleta() {
		if (esEscaleraCompleta())
			for (int i = 0; i < 13; i++)
				cartas.removeLast();
	}

	// Arma el texto de la columna dibujando cada carta oculta y cada carta visible.
	string mostrar() {
		int tamanoBocaArriba = tamano();
		int tamanoBocaAbajo = 0;

		if (vacia())
			return "";

		string texto = "";
		for (int i = 0; i < tamano(); i++) {
			Carta carta = (*this)[i];

			if (i > 0) {
				texto += "\n";
			}
			if (!carta.estaBocaArriba()) {
				texto += "┌───────┐  \n";
				texto += "│";
				texto += "\033[45m░░░░░░░";
				texto += "\033[0m│  ";
				tamanoBocaAbajo++;
			}
			else if (carta.estaBocaArriba() && i < tamano() - 1) {
				texto += dibujarCarta(to_string(tamanoBocaArriba - tamanoBocaAbajo), carta, false);
				tamanoBocaArriba--;
			}
			else {
				texto += dibujarCarta(to_string(tamanoBocaArriba - tamanoBocaAbajo), carta, true);
				tamanoBocaArriba--;
			}
		}
		return texto;
	}

	// Dibuja una carta boca arriba, completa cuando es el tope y recortada cuando tiene cartas encima.
	string dibujarCarta(string tamanoBocaArriba, Carta carta, bool completa) {
		string palo = (carta.getPalo() == 'E') ? "♠" : string(1, carta.getPalo());

		string cartaTexto = carta.toString();
		string valor = cartaTexto.substr(0, cartaTexto.find('-'));

		string texto = "┌───────┐  \n";

		texto += "│" + valor;
		for (int i = 0; i < 7 - valor.length(); i++)
			texto += " ";
		texto += "│" + tamanoBocaArriba;

		if (tamanoBocaArriba.length() == 1) {
			texto += " ";
		}

		if (completa) {
			texto += "\n│       │  \n";
			texto += "│   " + palo + "   │  \n";
			texto += "│       │  \n";

			texto += "│";
			for (int i = 0; i < 7 - valor.length(); i++)
				texto += " ";
			texto += valor + "│  \n";

			texto += "└───────┘  ";
		}
		return texto;
	}
};

#endif
