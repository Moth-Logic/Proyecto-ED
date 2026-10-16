#ifndef CARTA_H
#define CARTA_H

#include <string>

// Carta del mazo. Palo: 'C' corazones, 'D' diamantes, 'T' tréboles, 'E' espadas.
class Carta {
private:
	int valor;        // 1 = A ... 13 = K
	char palo;
	bool bocaArriba;

public:
	Carta() : valor(0), palo(' '), bocaArriba(false) {}
	Carta(int valor, char palo, bool bocaArriba = false)
		: valor(valor), palo(palo), bocaArriba(bocaArriba) {}

	int getValor() const { return valor; }
	char getPalo() const { return palo; }
	bool estaBocaArriba() const { return bocaArriba; }
	void voltear() { bocaArriba = !bocaArriba; }

	std::string toString() const {
		static const char* letras[] = { " ", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
		return std::string(valor >= 1 && valor <= 13 ? letras[valor] : "?") + '-' + palo;
	}
};

#endif
