#ifndef CARTA_H
#define CARTA_H

#include <string>

using std::string;

// Carta del mazo con valor (1=A ... 13=K), palo y estado boca arriba o boca abajo.
class Carta {
private:
	int valor;
	char palo;
	bool bocaArriba;

public:
	// Crea una carta vacía que se usa cuando no hay carta disponible.
	Carta() : valor(0), palo(' '), bocaArriba(false) {}

	// Crea la carta con su valor, palo y visibilidad inicial.
	Carta(int valor, char palo, bool bocaArriba = false)
		: valor(valor), palo(palo), bocaArriba(bocaArriba) {}

	// Devuelve el valor de la carta.
	int getValor() const { return valor; }
	// Devuelve el palo de la carta: 'C' corazones, 'D' diamantes, 'T' tréboles, 'E' espadas.
	char getPalo() const { return palo; }
	// Indica si la carta está boca arriba.
	bool estaBocaArriba() const { return bocaArriba; }
	// Cambia la carta de boca arriba a boca abajo y viceversa.
	void voltear() { bocaArriba = !bocaArriba; }

	// Arma el texto de la carta con el formato valor-palo.
	string toString() const {
		static const char* letras[] = { " ", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
		return string(valor >= 1 && valor <= 13 ? letras[valor] : "?") + '-' + palo;
	}
};

#endif
