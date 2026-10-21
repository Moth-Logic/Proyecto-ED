#ifndef MAZOGOLF_H
#define MAZOGOLF_H

#include <cstdlib>
#include <ctime>
#include "Pila.h"
#include "Carta.h"

// Mazo de 104 cartas de espadas (8 copias por valor) con 5 repartos de 10 cartas.
class MazoGolf {
private:
	Pila<Carta> cartas;
	int reparticionesRestantes;

	// Llena el mazo con las 104 cartas antes de barajarlo.
	void llenar() {
		for (int valor = 1; valor <= 13; valor++)
			for (int i = 0; i < 8; i++)
				cartas.push(Carta(valor, 'E'));
	}
}
